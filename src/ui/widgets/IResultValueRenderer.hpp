#ifndef IRESULTVALUERENDERER_HPP
#define IRESULTVALUERENDERER_HPP

#include "../../core/ModuleResult.hpp"
#include <QString>
#include <QWidget>
#include <memory>

/**
 * @brief Interface for rendering result values
 *
 * Allows extensible rendering of different ResultValue types.
 * Each renderer knows how to:
 * - Check if it can handle a value type
 * - Convert value to string representation
 * - Optionally create a detailed widget for complex types
 *
 * This enables adding new visualization types without modifying existing code.
 */
class IResultValueRenderer {
public:
    virtual ~IResultValueRenderer() = default;

    /**
     * @brief Check if this renderer can handle the given value
     * @param value The result value to check
     * @return true if this renderer can render the value
     */
    virtual bool canRender(const ResultValue& value) const = 0;

    /**
     * @brief Render the value as a string
     * @param key The key/name of the result
     * @param value The result value
     * @return String representation suitable for display
     */
    virtual QString render(const std::string& key, const ResultValue& value) const = 0;

    /**
     * @brief Create a detailed widget for complex visualizations (optional)
     *
     * For simple types (int, string), this returns nullptr.
     * For complex types (vectors, images), this can return a custom widget.
     *
     * @param key The key/name of the result
     * @param value The result value
     * @return Widget for detailed view, or nullptr for simple display
     */
    virtual QWidget* createDetailWidget(const std::string& key, const ResultValue& value) const {
        return nullptr; // Default: no detail widget
    }

    /**
     * @brief Get priority for this renderer (higher = checked first)
     *
     * Used when multiple renderers can handle the same type.
     * Default priority is 0.
     */
    virtual int getPriority() const {
        return 0;
    }
};

#endif // IRESULTVALUERENDERER_HPP
