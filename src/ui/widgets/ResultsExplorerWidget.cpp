#include "ResultsExplorerWidget.hpp"
#include <QHeaderView>
#include <QStyle>

ResultsExplorerWidget::ResultsExplorerWidget(QWidget *parent)
    : QDockWidget("Results Explorer", parent)
    , displayManager_(std::make_unique<ResultDisplayManager>())
{
    // Create main widget
    QWidget* mainWidget = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(mainWidget);

    // Summary label
    summaryLabel_ = new QLabel("No results yet");
    summaryLabel_->setStyleSheet("QLabel { font-weight: bold; padding: 5px; }");
    layout->addWidget(summaryLabel_);

    // Tree widget
    treeWidget_ = new QTreeWidget();
    treeWidget_->setHeaderLabels({"Result", "Value"});
    treeWidget_->setAlternatingRowColors(true);
    treeWidget_->setColumnWidth(0, 250);
    treeWidget_->setColumnWidth(1, 150);
    layout->addWidget(treeWidget_);

    // Clear button
    clearButton_ = new QPushButton("Clear Results");
    connect(clearButton_, &QPushButton::clicked, this, &ResultsExplorerWidget::clear);
    layout->addWidget(clearButton_);

    setWidget(mainWidget);

    // Dock widget settings
    setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    setFeatures(QDockWidget::DockWidgetMovable |
                QDockWidget::DockWidgetFloatable |
                QDockWidget::DockWidgetClosable);
}

void ResultsExplorerWidget::displayResults(const std::vector<ModuleExecutionResult>& results)
{
    clear();

    for (const auto& result : results) {
        QTreeWidgetItem* moduleItem = createModuleItem(result);
        treeWidget_->addTopLevelItem(moduleItem);

        // Add result values
        addResultValues(moduleItem, result.result);

        // Add errors/warnings
        addIssues(moduleItem, result.result);
    }

    // Expand all by default
    treeWidget_->expandAll();

    // Update summary
    updateSummary(results);
}

void ResultsExplorerWidget::clear()
{
    treeWidget_->clear();
    summaryLabel_->setText("No results yet");
}

QTreeWidgetItem* ResultsExplorerWidget::createModuleItem(const ModuleExecutionResult& result)
{
    QTreeWidgetItem* item = new QTreeWidgetItem();

    // Set module name
    item->setText(0, QString::fromStdString(result.displayName.empty() ? result.moduleName : result.displayName));

    // Set execution time
    item->setText(1, QString::number(result.duration.count()) + " ms");

    // Set icon based on status
    item->setIcon(0, getStatusIcon(result.getStatusIcon()));

    // Make it bold
    QFont font = item->font(0);
    font.setBold(true);
    item->setFont(0, font);

    // Color based on status
    if (!result.isSuccess() || result.hasErrors()) {
        item->setForeground(0, QBrush(QColor(200, 0, 0)));
    } else if (result.hasWarnings()) {
        item->setForeground(0, QBrush(QColor(200, 100, 0)));
    } else {
        item->setForeground(0, QBrush(QColor(0, 150, 0)));
    }

    return item;
}

void ResultsExplorerWidget::addResultValues(QTreeWidgetItem* moduleItem, const ModuleResult& result)
{
    if (result.values.empty()) {
        return;
    }

    // Create "Results" category
    QTreeWidgetItem* resultsCategory = new QTreeWidgetItem(moduleItem);
    resultsCategory->setText(0, "📊 Results");
    QFont font = resultsCategory->font(0);
    font.setItalic(true);
    resultsCategory->setFont(0, font);

    // Add each result value
    for (const auto& [key, value] : result.values) {
        QTreeWidgetItem* valueItem = new QTreeWidgetItem(resultsCategory);

        // Use display manager to render the value
        QString rendered = displayManager_->render(key, value);

        // Split into key and value for columns
        int colonPos = rendered.indexOf(": ");
        if (colonPos != -1) {
            valueItem->setText(0, rendered.left(colonPos));
            valueItem->setText(1, rendered.mid(colonPos + 2));
        } else {
            valueItem->setText(0, rendered);
        }
    }
}

void ResultsExplorerWidget::addIssues(QTreeWidgetItem* moduleItem, const ModuleResult& result)
{
    if (result.issues.empty()) {
        // Add message if present
        if (!result.message.empty()) {
            QTreeWidgetItem* messageItem = new QTreeWidgetItem(moduleItem);
            messageItem->setText(0, "💬 Message");
            messageItem->setText(1, QString::fromStdString(result.message));
            messageItem->setForeground(1, QBrush(QColor(100, 100, 100)));
        }
        return;
    }

    // Create "Issues" category
    QTreeWidgetItem* issuesCategory = new QTreeWidgetItem(moduleItem);

    int errorCount = 0;
    int warningCount = 0;

    for (const auto& issue : result.issues) {
        if (issue.type == ErrorType::Error || issue.type == ErrorType::Critical) {
            errorCount++;
        } else if (issue.type == ErrorType::Warning) {
            warningCount++;
        }
    }

    QString categoryText = "⚠️ Issues";
    if (errorCount > 0 && warningCount > 0) {
        categoryText += QString(" (%1 error%2, %3 warning%4)")
        .arg(errorCount).arg(errorCount > 1 ? "s" : "")
            .arg(warningCount).arg(warningCount > 1 ? "s" : "");
    } else if (errorCount > 0) {
        categoryText += QString(" (%1 error%2)")
        .arg(errorCount).arg(errorCount > 1 ? "s" : "");
    } else if (warningCount > 0) {
        categoryText += QString(" (%1 warning%2)")
        .arg(warningCount).arg(warningCount > 1 ? "s" : "");
    }

    issuesCategory->setText(0, categoryText);
    QFont font = issuesCategory->font(0);
    font.setItalic(true);
    issuesCategory->setFont(0, font);

    // Add each issue
    for (const auto& issue : result.issues) {
        QTreeWidgetItem* issueItem = new QTreeWidgetItem(issuesCategory);

        QString prefix;
        QColor color;

        switch (issue.type) {
        case ErrorType::Critical:
        case ErrorType::Error:
            prefix = "❌ ";
            color = QColor(200, 0, 0);
            break;
        case ErrorType::Warning:
            prefix = "⚠️ ";
            color = QColor(200, 100, 0);
            break;
        case ErrorType::Info:
            prefix = "ℹ️ ";
            color = QColor(0, 100, 200);
            break;
        }

        issueItem->setText(0, prefix + QString::fromStdString(issue.code));
        issueItem->setText(1, QString::fromStdString(issue.message));
        issueItem->setForeground(0, QBrush(color));
        issueItem->setForeground(1, QBrush(color));
    }

    // Add message if present
    if (!result.message.empty()) {
        QTreeWidgetItem* messageItem = new QTreeWidgetItem(moduleItem);
        messageItem->setText(0, "💬 Message");
        messageItem->setText(1, QString::fromStdString(result.message));
        messageItem->setForeground(1, QBrush(QColor(100, 100, 100)));
    }
}

void ResultsExplorerWidget::updateSummary(const std::vector<ModuleExecutionResult>& results)
{
    if (results.empty()) {
        summaryLabel_->setText("No modules executed");
        return;
    }

    int successCount = 0;
    int errorCount = 0;
    int warningCount = 0;

    for (const auto& result : results) {
        if (result.isSuccess() && !result.hasErrors()) {
            successCount++;
        }
        if (result.hasErrors()) {
            errorCount++;
        }
        if (result.hasWarnings()) {
            warningCount++;
        }
    }

    QString summary = QString("📊 %1 module%2 executed: ")
                          .arg(results.size())
                          .arg(results.size() > 1 ? "s" : "");

    summary += QString("✅ %1 success").arg(successCount);

    if (errorCount > 0) {
        summary += QString(", ❌ %1 error%2").arg(errorCount).arg(errorCount > 1 ? "s" : "");
    }

    if (warningCount > 0) {
        summary += QString(", ⚠️ %1 warning%2").arg(warningCount).arg(warningCount > 1 ? "s" : "");
    }

    summaryLabel_->setText(summary);
}

QIcon ResultsExplorerWidget::getStatusIcon(const std::string& status) const
{
    if (status == "success") {
        return style()->standardIcon(QStyle::SP_DialogApplyButton);
    } else if (status == "warning") {
        return style()->standardIcon(QStyle::SP_MessageBoxWarning);
    } else {
        return style()->standardIcon(QStyle::SP_MessageBoxCritical);
    }
}
