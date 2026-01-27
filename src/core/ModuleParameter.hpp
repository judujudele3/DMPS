#ifndef MODULEPARAMETER_HPP
#define MODULEPARAMETER_HPP

#include <string>
#include <variant>
#include <optional>
#include <map>

/**
 * @brief Structure representing a single parameter that a module needs
 *
 * This is used to describe module parameters in a declarative way,
 * allowing the UI to automatically generate configuration dialogs.
 *
 * Example:
 * {"radius", "int", 5, "Blur radius in pixels", 1, 50}
 */
struct ModuleParameter {
    std::string name;           // Parameter name (e.g., "radius", "column")
    std::string type;           // Type: "int", "float", "string", "bool"

    // Default value (can be any supported type)
    std::variant<int, float, std::string, bool> defaultValue;

    std::string description;    // Human-readable description for UI tooltips

    // Optional constraints for numeric types
    std::optional<int> minInt;      // Minimum value for int
    std::optional<int> maxInt;      // Maximum value for int
    std::optional<float> minFloat;  // Minimum value for float
    std::optional<float> maxFloat;  // Maximum value for float

    /**
     * @brief Constructor for parameters without constraints
     */
    ModuleParameter(const std::string& name,
                    const std::string& type,
                    const std::variant<int, float, std::string, bool>& defaultValue,
                    const std::string& description = "")
        : name(name)
        , type(type)
        , defaultValue(defaultValue)
        , description(description)
    {}

    /**
     * @brief Constructor for int parameters with min/max constraints
     */
    ModuleParameter(const std::string& name,
                    const std::string& type,
                    int defaultValue,
                    const std::string& description,
                    int minValue,
                    int maxValue)
        : name(name)
        , type(type)
        , defaultValue(defaultValue)
        , description(description)
        , minInt(minValue)
        , maxInt(maxValue)
    {}

    /**
     * @brief Constructor for float parameters with min/max constraints
     */
    ModuleParameter(const std::string& name,
                    const std::string& type,
                    float defaultValue,
                    const std::string& description,
                    float minValue,
                    float maxValue)
        : name(name)
        , type(type)
        , defaultValue(defaultValue)
        , description(description)
        , minFloat(minValue)
        , maxFloat(maxValue)
    {}

    /**
     * @brief Check if this parameter has constraints
     */
    bool hasConstraints() const {
        return minInt.has_value() || maxInt.has_value() ||
               minFloat.has_value() || maxFloat.has_value();
    }

    /**
     * @brief Validate a value against constraints
     * @return true if valid or no constraints, false otherwise
     */
    bool isValid(const std::variant<int, float, std::string, bool>& value) const {
        if (type == "int" && std::holds_alternative<int>(value)) {
            int v = std::get<int>(value);
            if (minInt.has_value() && v < minInt.value()) return false;
            if (maxInt.has_value() && v > maxInt.value()) return false;
        }
        else if (type == "float" && std::holds_alternative<float>(value)) {
            float v = std::get<float>(value);
            if (minFloat.has_value() && v < minFloat.value()) return false;
            if (maxFloat.has_value() && v > maxFloat.value()) return false;
        }
        return true;
    }
};

// Type alias for parameter values (used in ModuleManager and UI)
using ParamValue = std::variant<int, float, std::string, bool>;
using ParamMap = std::map<std::string, ParamValue>;

#endif // MODULEPARAMETER_HPP
