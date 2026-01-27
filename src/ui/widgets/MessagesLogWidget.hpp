#ifndef MESSAGESLOGWIDGET_HPP
#define MESSAGESLOGWIDGET_HPP

#include <QDockWidget>
#include <QPlainTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QCheckBox>
#include "../../core/ModuleExecutionResult.hpp"
#include <vector>

/**
 * @brief Widget for displaying execution logs in a terminal-like format
 *
 * Features:
 * - Chronological display of module execution
 * - Color-coded messages (errors in red, success in green, etc.)
 * - Auto-scroll to bottom
 * - Clear button
 * - Filter options (show/hide warnings, errors, etc.)
 */
class MessagesLogWidget : public QDockWidget
{
    Q_OBJECT

public:
    explicit MessagesLogWidget(QWidget *parent = nullptr);
    ~MessagesLogWidget() = default;

    /**
     * @brief Log module execution results
     * @param results Vector of module execution results
     */
    void logResults(const std::vector<ModuleExecutionResult>& results);

    /**
     * @brief Log a custom message
     * @param message The message to log
     * @param isError Whether this is an error message
     */
    void logMessage(const QString& message, bool isError = false);

    /**
     * @brief Clear all logs
     */
    void clear();

private:
    QPlainTextEdit* logTextEdit_;
    QPushButton* clearButton_;
    QCheckBox* autoScrollCheckBox_;

    /**
     * @brief Append colored HTML text to log
     * @param text The text to append
     * @param color Color name or hex code
     */
    void appendColoredText(const QString& text, const QString& color);

    /**
     * @brief Get current timestamp string
     */
    QString getTimestamp() const;

    /**
     * @brief Log a single module result
     */
    void logModuleResult(const ModuleExecutionResult& result);

    /**
     * @brief Auto-scroll to bottom if enabled
     */
    void autoScroll();
};

#endif // MESSAGESLOGWIDGET_HPP
