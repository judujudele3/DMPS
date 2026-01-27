#include "MessagesLogWidget.hpp"
#include <QScrollBar>
#include <QDateTime>

MessagesLogWidget::MessagesLogWidget(QWidget *parent)
    : QDockWidget("Messages & Logs", parent)
{
    // Create main widget
    QWidget* mainWidget = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(mainWidget);
    layout->setContentsMargins(0, 0, 0, 0);

    // Log text edit
    logTextEdit_ = new QPlainTextEdit();
    logTextEdit_->setReadOnly(true);
    logTextEdit_->setMaximumBlockCount(1000); // Limit to 1000 lines

    // Font style (monospaced for log-like appearance)
    QFont font("Courier");
    font.setPointSize(9);
    logTextEdit_->setFont(font);

    // Dark background like a terminal
    logTextEdit_->setStyleSheet(
        "QPlainTextEdit {"
        "  background-color: #1e1e1e;"
        "  color: #d4d4d4;"
        "  border: none;"
        "}"
        );

    layout->addWidget(logTextEdit_);

    // Bottom toolbar
    QWidget* toolbar = new QWidget();
    QHBoxLayout* toolbarLayout = new QHBoxLayout(toolbar);
    toolbarLayout->setContentsMargins(5, 5, 5, 5);

    clearButton_ = new QPushButton("Clear");
    clearButton_->setMaximumWidth(80);
    connect(clearButton_, &QPushButton::clicked, this, &MessagesLogWidget::clear);
    toolbarLayout->addWidget(clearButton_);

    autoScrollCheckBox_ = new QCheckBox("Auto-scroll");
    autoScrollCheckBox_->setChecked(true);
    toolbarLayout->addWidget(autoScrollCheckBox_);

    toolbarLayout->addStretch();

    layout->addWidget(toolbar);

    setWidget(mainWidget);

    // Dock widget settings
    setAllowedAreas(Qt::BottomDockWidgetArea | Qt::TopDockWidgetArea);
    setFeatures(QDockWidget::DockWidgetMovable |
                QDockWidget::DockWidgetFloatable |
                QDockWidget::DockWidgetClosable);

    // Initial message
    appendColoredText("=== Module Execution Log ===", "#808080");
    appendColoredText("Ready to execute modules.", "#808080");
}

void MessagesLogWidget::logResults(const std::vector<ModuleExecutionResult>& results)
{
    if (results.empty()) {
        logMessage("No modules to execute.", true);
        return;
    }

    appendColoredText("\n" + getTimestamp() + " Applying " +
                          QString::number(results.size()) + " module" +
                          (results.size() > 1 ? "s" : "") + "...", "#4ec9b0");

    for (const auto& result : results) {
        logModuleResult(result);
    }

    // Summary
    int successCount = 0;
    int errorCount = 0;

    for (const auto& result : results) {
        if (result.isSuccess() && !result.hasErrors()) {
            successCount++;
        } else {
            errorCount++;
        }
    }

    QString summaryColor = (errorCount > 0) ? "#f48771" : "#4ec9b0";
    appendColoredText(getTimestamp() + " Summary: " +
                          QString::number(successCount) + "/" +
                          QString::number(results.size()) + " modules succeeded",
                      summaryColor);

    autoScroll();
}

void MessagesLogWidget::logMessage(const QString& message, bool isError)
{
    QString color = isError ? "#f48771" : "#d4d4d4";
    appendColoredText(getTimestamp() + " " + message, color);
    autoScroll();
}

void MessagesLogWidget::clear()
{
    logTextEdit_->clear();
    appendColoredText("=== Module Execution Log ===", "#808080");
    appendColoredText("Log cleared.", "#808080");
}

void MessagesLogWidget::appendColoredText(const QString& text, const QString& color)
{
    // Use HTML for colored text
    QString html = QString("<span style='color: %1;'>%2</span>")
                       .arg(color)
                       .arg(text.toHtmlEscaped());

    logTextEdit_->appendHtml(html);
}

QString MessagesLogWidget::getTimestamp() const
{
    return "[" + QDateTime::currentDateTime().toString("HH:mm:ss") + "]";
}

void MessagesLogWidget::logModuleResult(const ModuleExecutionResult& result)
{
    QString moduleName = QString::fromStdString(
        result.displayName.empty() ? result.moduleName : result.displayName
        );

    if (result.isSuccess() && !result.hasErrors()) {
        // Success
        appendColoredText(getTimestamp() + " ✅ " + moduleName + ": Completed in " +
                              QString::number(result.duration.count()) + "ms", "#4ec9b0");

        // Log message if present
        if (!result.result.message.empty()) {
            appendColoredText("   → " + QString::fromStdString(result.result.message), "#808080");
        }

        // Log key results (first 3)
        int count = 0;
        for (const auto& [key, value] : result.result.values) {
            if (count >= 3) {
                appendColoredText("   → ... (" +
                                      QString::number(result.result.values.size() - 3) +
                                      " more results)", "#808080");
                break;
            }

            // Simple display for logs (just show key name)
            appendColoredText("   → " + QString::fromStdString(key) + " computed", "#569cd6");
            count++;
        }
    } else {
        // Error or warning
        QString statusIcon = result.hasErrors() ? "❌" : "⚠️";
        QString statusColor = result.hasErrors() ? "#f48771" : "#ce9178";

        appendColoredText(getTimestamp() + " " + statusIcon + " " + moduleName + ": " +
                              (result.hasErrors() ? "Failed" : "Completed with warnings"),
                          statusColor);

        // Log issues
        for (const auto& issue : result.result.issues) {
            QString issueColor;
            QString issuePrefix;

            switch (issue.type) {
            case ErrorType::Critical:
            case ErrorType::Error:
                issueColor = "#f48771";
                issuePrefix = "   → Error: ";
                break;
            case ErrorType::Warning:
                issueColor = "#ce9178";
                issuePrefix = "   → Warning: ";
                break;
            case ErrorType::Info:
                issueColor = "#569cd6";
                issuePrefix = "   → Info: ";
                break;
            }

            appendColoredText(issuePrefix + QString::fromStdString(issue.code) +
                                  " - " + QString::fromStdString(issue.message), issueColor);
        }
    }
}

void MessagesLogWidget::autoScroll()
{
    if (autoScrollCheckBox_->isChecked()) {
        QScrollBar* scrollBar = logTextEdit_->verticalScrollBar();
        scrollBar->setValue(scrollBar->maximum());
    }
}
