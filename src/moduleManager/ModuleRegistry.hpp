#ifndef MODULEREGISTRY_HPP
#define MODULEREGISTRY_HPP

#include "../core/ModuleParameter.hpp"
#include "../module/IModule.hpp"
#include <string>
#include <vector>
#include <functional>
#include <memory>

/**
 * @brief Information about a module available in the system
 *
 * Contains everything needed to:
 * - Display the module in the UI
 * - Know what parameters it needs
 * - Create instances of the module
 */
struct ModuleInfo {
    std::string name;        // Unique identifier (e.g., "BlurModule")
    std::string displayName; // Human-readable name (e.g., "Blur Filter")
    std::string category;    // Category for UI organization (e.g., "Image Processing")

    std::vector<ModuleParameter> parameters; // Parameters this module needs

    // Factory function that creates a module instance with given parameters
    std::function<std::shared_ptr<IModule>(const ParamMap&)> factory;

    /**
     * @brief Check if this module requires parameters
     */
    bool hasParameters() const {
        return !parameters.empty();
    }
};

/**
 * @brief Central registry of all available modules in the system
 *
 * This is where all modules are registered at startup.
 * The UI queries this to show available modules.
 * The ModuleManager uses this to create module instances.
 *
 * Design: Static registry (modules known at compile time)
 * Future: Could be extended to support dynamic plugin loading
 */
class ModuleRegistry {
public:
    /**
     * @brief Get the list of all available modules
     * @return Reference to the static list of module info
     */
    static const std::vector<ModuleInfo>& getAvailableModules();

    /**
     * @brief Find a module by its name
     * @param name The unique name of the module
     * @return Pointer to ModuleInfo if found, nullptr otherwise
     */
    static const ModuleInfo* findModule(const std::string& name);

    /**
     * @brief Get all modules in a specific category
     * @param category The category name
     * @return Vector of ModuleInfo matching the category
     */
    static std::vector<const ModuleInfo*> getModulesByCategory(const std::string& category);

    /**
     * @brief Get all unique categories
     * @return Vector of category names
     */
    static std::vector<std::string> getCategories();

private:
    // Private constructor - this is a static-only class
    ModuleRegistry() = delete;

    // Internal helper to initialize the registry
    static std::vector<ModuleInfo> initializeRegistry();
};

#endif // MODULEREGISTRY_HPP
