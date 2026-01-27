#include "ModuleConfigDialog.hpp"
#include "ui_ModuleConfigDialog.h"
#include "ModuleParameterDialog.hpp"
#include <QMessageBox>

ModuleConfigDialog::ModuleConfigDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ModuleConfigDialog)
{
    ui->setupUi(this);

    // Configure tree widget
    ui->treeWidgetModules->setColumnWidth(0, 300);
    ui->treeWidgetModules->setColumnWidth(1, 250);

    // Connect signals
    connect(ui->treeWidgetModules, &QTreeWidget::itemChanged,
            this, &ModuleConfigDialog::onModuleItemChanged);
    connect(ui->treeWidgetModules, &QTreeWidget::itemSelectionChanged,
            this, &ModuleConfigDialog::onModuleSelectionChanged);
    connect(ui->pushButtonConfigure, &QPushButton::clicked,
            this, &ModuleConfigDialog::onConfigureClicked);
}

ModuleConfigDialog::~ModuleConfigDialog()
{
    delete ui;
}

void ModuleConfigDialog::setAvailableModules(const std::vector<ModuleInfo>& modules)
{
    availableModules_ = modules;
    buildModuleTree(modules);
}

void ModuleConfigDialog::setActiveModules(const std::vector<SelectedModule>& activeModules)
{
    // Pre-check the modules that are already active
    for (const auto& selected : activeModules) {
        // Find the corresponding item in the tree
        QTreeWidgetItemIterator it(ui->treeWidgetModules);
        while (*it) {
            if (isModuleItem(*it)) {
                std::string moduleName = getModuleName(*it);
                if (moduleName == selected.moduleName) {
                    (*it)->setCheckState(0, Qt::Checked);
                    // Store the configured parameters
                    configuredParameters_[moduleName] = selected.parameters;
                    break;
                }
            }
            ++it;
        }
    }

    updateStatusLabel();
}

std::vector<SelectedModule> ModuleConfigDialog::getSelectedModules() const
{
    std::vector<SelectedModule> result;

    QTreeWidgetItemIterator it(ui->treeWidgetModules);
    while (*it) {
        if (isModuleItem(*it) && (*it)->checkState(0) == Qt::Checked) {
            std::string moduleName = getModuleName(*it);

            // Get configured parameters or use defaults
            ParamMap params;
            auto configIt = configuredParameters_.find(moduleName);
            if (configIt != configuredParameters_.end()) {
                params = configIt->second;
            } else {
                // Use default parameters
                const ModuleInfo* info = getModuleInfo(*it);
                if (info) {
                    for (const auto& param : info->parameters) {
                        params[param.name] = param.defaultValue;
                    }
                }
            }

            result.push_back(SelectedModule(moduleName, params));
        }
        ++it;
    }

    return result;
}

void ModuleConfigDialog::onModuleItemChanged(QTreeWidgetItem* item, int column)
{
    if (column == 0 && isModuleItem(item)) {
        updateStatusLabel();

        // If module was just checked and has parameters,
        // we could auto-open config dialog here (but we chose not to, per your preference)
    }
}

void ModuleConfigDialog::onModuleSelectionChanged()
{
    // Enable/disable Configure button based on selection
    QTreeWidgetItem* selected = ui->treeWidgetModules->currentItem();

    bool enableConfigure = false;
    if (selected && isModuleItem(selected)) {
        // Only enable if the module is checked AND has parameters
        if (selected->checkState(0) == Qt::Checked) {
            const ModuleInfo* info = getModuleInfo(selected);
            if (info && info->hasParameters()) {
                enableConfigure = true;
            }
        }
    }

    ui->pushButtonConfigure->setEnabled(enableConfigure);
}

void ModuleConfigDialog::onConfigureClicked()
{
    QTreeWidgetItem* selected = ui->treeWidgetModules->currentItem();
    if (!selected || !isModuleItem(selected)) {
        return;
    }

    const ModuleInfo* info = getModuleInfo(selected);
    if (!info || !info->hasParameters()) {
        return;
    }

    std::string moduleName = getModuleName(selected);

    // Get current parameters (either configured or defaults)
    ParamMap currentParams;
    auto it = configuredParameters_.find(moduleName);
    if (it != configuredParameters_.end()) {
        currentParams = it->second;
    } else {
        // Use defaults
        for (const auto& param : info->parameters) {
            currentParams[param.name] = param.defaultValue;
        }
    }

    // Open parameter configuration dialog
    ModuleParameterDialog paramDialog(info->displayName, info->parameters, currentParams, this);

    if (paramDialog.exec() == QDialog::Accepted) {
        // Store the configured parameters
        configuredParameters_[moduleName] = paramDialog.getParameters();
    }
}

void ModuleConfigDialog::buildModuleTree(const std::vector<ModuleInfo>& modules)
{
    ui->treeWidgetModules->clear();
    configuredParameters_.clear();

    // Block signals while building to avoid triggering itemChanged
    ui->treeWidgetModules->blockSignals(true);

    for (const auto& module : modules) {
        QTreeWidgetItem* categoryItem = findOrCreateCategory(QString::fromStdString(module.category));
        QTreeWidgetItem* moduleItem = createModuleItem(module);
        categoryItem->addChild(moduleItem);
    }

    // Expand all categories
    ui->treeWidgetModules->expandAll();

    ui->treeWidgetModules->blockSignals(false);

    updateStatusLabel();
}

QTreeWidgetItem* ModuleConfigDialog::findOrCreateCategory(const QString& categoryName)
{
    // Search for existing category
    for (int i = 0; i < ui->treeWidgetModules->topLevelItemCount(); ++i) {
        QTreeWidgetItem* item = ui->treeWidgetModules->topLevelItem(i);
        if (item->text(0) == categoryName) {
            return item;
        }
    }

    // Create new category
    QTreeWidgetItem* categoryItem = new QTreeWidgetItem();
    categoryItem->setText(0, categoryName);
    categoryItem->setFlags(Qt::ItemIsEnabled); // Not checkable, not selectable

    // Make category bold
    QFont font = categoryItem->font(0);
    font.setBold(true);
    categoryItem->setFont(0, font);

    // Add icon (folder icon)
    categoryItem->setIcon(0, style()->standardIcon(QStyle::SP_DirIcon));

    ui->treeWidgetModules->addTopLevelItem(categoryItem);

    return categoryItem;
}

QTreeWidgetItem* ModuleConfigDialog::createModuleItem(const ModuleInfo& moduleInfo)
{
    QTreeWidgetItem* item = new QTreeWidgetItem();

    // Set module name and description
    item->setText(0, QString::fromStdString(moduleInfo.displayName));

    // Build description text
    QString description;
    if (moduleInfo.hasParameters()) {
        description = QString("(%1 parameter%2)")
        .arg(moduleInfo.parameters.size())
            .arg(moduleInfo.parameters.size() > 1 ? "s" : "");
    } else {
        description = "(no parameters)";
    }
    item->setText(1, description);

    // Make it checkable
    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable);
    item->setCheckState(0, Qt::Unchecked);

    // Store module name in user data for easy retrieval
    item->setData(0, Qt::UserRole, QString::fromStdString(moduleInfo.name));

    return item;
}

void ModuleConfigDialog::updateStatusLabel()
{
    int count = countCheckedModules();
    ui->labelStatus->setText(QString("%1 module%2 selected")
                                 .arg(count)
                                 .arg(count == 1 ? "" : "s"));
}

bool ModuleConfigDialog::isModuleItem(QTreeWidgetItem* item) const
{
    // Module items have user data set (the module name)
    return item && !item->data(0, Qt::UserRole).toString().isEmpty();
}

std::string ModuleConfigDialog::getModuleName(QTreeWidgetItem* item) const
{
    if (!isModuleItem(item)) {
        return "";
    }
    return item->data(0, Qt::UserRole).toString().toStdString();
}

const ModuleInfo* ModuleConfigDialog::getModuleInfo(QTreeWidgetItem* item) const
{
    std::string moduleName = getModuleName(item);
    if (moduleName.empty()) {
        return nullptr;
    }

    for (const auto& module : availableModules_) {
        if (module.name == moduleName) {
            return &module;
        }
    }

    return nullptr;
}

int ModuleConfigDialog::countCheckedModules() const
{
    int count = 0;
    QTreeWidgetItemIterator it(ui->treeWidgetModules);
    while (*it) {
        if (isModuleItem(*it) && (*it)->checkState(0) == Qt::Checked) {
            count++;
        }
        ++it;
    }
    return count;
}
