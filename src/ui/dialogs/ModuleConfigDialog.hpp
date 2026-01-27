#ifndef MODULECONFIGDIALOG_HPP
#define MODULECONFIGDIALOG_HPP

#include <QDialog>
#include <QTreeWidgetItem>
#include <vector>
#include <map>
#include "../../moduleManager/ModuleRegistry.hpp"
#include "../../moduleManager/ModuleManager.hpp"

namespace Ui {
class ModuleConfigDialog;
}

/**
 * @brief Dialog for enabling/disabling and configuring modules
 *
 * This dialog shows a tree of available modules organized by category.
 * Users can:
 * - Check/uncheck modules to enable/disable them
 * - Select a checked module and click "Configure" to set parameters
 * - Click OK to apply the configuration
 *
 * The dialog maintains the state of:
 * - Which modules are checked
 * - What parameters each module has been configured with
 */
class ModuleConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ModuleConfigDialog(QWidget *parent = nullptr);
    ~ModuleConfigDialog();

    /**
     * @brief Set the list of available modules (from Registry)
     * @param modules Vector of all available modules
     */
    void setAvailableModules(const std::vector<ModuleInfo>& modules);

    /**
     * @brief Set the currently active modules (to pre-check them)
     * @param activeModules Vector of modules that are currently active
     */
    void setActiveModules(const std::vector<SelectedModule>& activeModules);

    /**
     * @brief Get the list of selected modules with their parameters
     * @return Vector of SelectedModule (only checked modules)
     */
    std::vector<SelectedModule> getSelectedModules() const;

private slots:
    /**
     * @brief Called when a module is checked/unchecked
     */
    void onModuleItemChanged(QTreeWidgetItem* item, int column);

    /**
     * @brief Called when selection changes in the tree
     */
    void onModuleSelectionChanged();

    /**
     * @brief Called when Configure button is clicked
     */
    void onConfigureClicked();

private:
    Ui::ModuleConfigDialog *ui;

    // Map to store configured parameters for each module
    // Key: module name, Value: configured parameters
    std::map<std::string, ParamMap> configuredParameters_;

    /**
     * @brief Build the tree structure from available modules
     */
    void buildModuleTree(const std::vector<ModuleInfo>& modules);

    /**
     * @brief Find or create a category item in the tree
     * @param categoryName Name of the category
     * @return Pointer to the category tree item
     */
    QTreeWidgetItem* findOrCreateCategory(const QString& categoryName);

    /**
     * @brief Create a tree item for a module
     * @param moduleInfo Information about the module
     * @return Pointer to the created tree item
     */
    QTreeWidgetItem* createModuleItem(const ModuleInfo& moduleInfo);

    /**
     * @brief Update the status label with count of selected modules
     */
    void updateStatusLabel();

    /**
     * @brief Check if an item is a module (not a category)
     * @param item Tree widget item to check
     * @return true if it's a module item, false if category
     */
    bool isModuleItem(QTreeWidgetItem* item) const;

    /**
     * @brief Get module name from a tree item
     * @param item Tree widget item
     * @return Module name or empty string if not a module item
     */
    std::string getModuleName(QTreeWidgetItem* item) const;

    /**
     * @brief Get module info for a given item
     * @param item Tree widget item
     * @return Pointer to ModuleInfo or nullptr if not found
     */
    const ModuleInfo* getModuleInfo(QTreeWidgetItem* item) const;

    /**
     * @brief Count the number of checked modules
     */
    int countCheckedModules() const;

    // Store available modules for reference
    std::vector<ModuleInfo> availableModules_;
};

#endif // MODULECONFIGDIALOG_HPP
