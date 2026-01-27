#ifndef RESULTSEXPLORERWIDGET_HPP
#define RESULTSEXPLORERWIDGET_HPP

#include <QDockWidget>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include "../../core/ModuleExecutionResult.hpp"
#include "ResultDisplayManager.hpp"
#include <vector>
#include <memory>

/**
 * @brief Widget for displaying module execution results in a tree view
 *
 * Displays:
 * - Module name with status icon (✅ ❌ ⚠️)
 * - Result values as child items
 * - Error/warning details
 *
 * Can be docked/undocked/resized.
 */
class ResultsExplorerWidget : public QDockWidget
{
    Q_OBJECT

public:
    explicit ResultsExplorerWidget(QWidget *parent = nullptr);
    ~ResultsExplorerWidget() = default;

    /**
     * @brief Display results from module execution
     * @param results Vector of module execution results
     */
    void displayResults(const std::vector<ModuleExecutionResult>& results);

    /**
     * @brief Clear all displayed results
     */
    void clear();

private:
    QTreeWidget* treeWidget_;
    QLabel* summaryLabel_;
    QPushButton* clearButton_;

    std::unique_ptr<ResultDisplayManager> displayManager_;

    /**
     * @brief Create a tree item for a module result
     * @param result The module execution result
     * @return Tree widget item
     */
    QTreeWidgetItem* createModuleItem(const ModuleExecutionResult& result);

    /**
     * @brief Add result values as children to module item
     * @param moduleItem The parent module item
     * @param result The module result
     */
    void addResultValues(QTreeWidgetItem* moduleItem, const ModuleResult& result);

    /**
     * @brief Add errors/warnings as children to module item
     * @param moduleItem The parent module item
     * @param result The module result
     */
    void addIssues(QTreeWidgetItem* moduleItem, const ModuleResult& result);

    /**
     * @brief Update the summary label
     * @param results Vector of module execution results
     */
    void updateSummary(const std::vector<ModuleExecutionResult>& results);

    /**
     * @brief Get icon for status
     */
    QIcon getStatusIcon(const std::string& status) const;
};
#endif // RESULTSEXPLORERWIDGET_HPP
