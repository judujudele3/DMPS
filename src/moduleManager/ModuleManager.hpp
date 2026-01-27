#ifndef MODULEMANAGER_HPP
#define MODULEMANAGER_HPP

#include "../core/ModuleParameter.hpp"
#include "ModuleRegistry.hpp"
#include "../module/IModule.hpp"
#include <vector>
#include <memory>
#include <string>

/**
 * @brief Represents a module selected by the user with its configured parameters
 *
 * This is the data structure passed from the UI to the backend
 * when the user has chosen which modules to activate and configured them.
 */
struct SelectedModule {
    std::string moduleName;     // Name of the module (matches ModuleInfo::name)
    ParamMap parameters;        // Configured parameter values

    SelectedModule(const std::string& name, const ParamMap& params = {})
        : moduleName(name), parameters(params) {}
};

/**
 * @brief Manages the lifecycle of active modules
 *
 * Responsibilities:
 * - Creates module instances from user selections
 * - Maintains the list of active modules
 * - Validates parameters before construction
 * - Provides the active module list to the Engine
 *
 * This class bridges the gap between:
 * - UI selections (SelectedModule objects)
 * - Actual module instances (IModule objects)
 */
class ModuleManager {
public:
    ModuleManager();
    ~ModuleManager() = default;

    /**
     * @brief Update the active modules based on user selection
     *
     * This method:
     * 1. Clears the current active modules
     * 2. For each selected module:
     *    - Finds it in the registry
     *    - Validates parameters
     *    - Creates an instance via factory
     * 3. Stores the instances
     *
     * @param selectedModules List of modules chosen by user with their parameters
     * @return true if all modules were created successfully, false otherwise
     */
    bool updateActiveModules(const std::vector<SelectedModule>& selectedModules);

    /**
     * @brief Get the list of currently active module instances
     *
     * This is what gets passed to the Engine for execution.
     *
     * @return Reference to the vector of active modules
     */
    const std::vector<std::shared_ptr<IModule>>& getActiveModuleInstances() const;

    /**
     * @brief Get information about currently active modules
     *
     * Useful for UI display (showing which modules are active).
     *
     * @return Vector of SelectedModule representing current state
     */
    std::vector<SelectedModule> getActiveModulesInfo() const;

    /**
     * @brief Clear all active modules
     */
    void clearActiveModules();

    /**
     * @brief Get the number of active modules
     */
    size_t getActiveModuleCount() const;

    /**
     * @brief Check if a specific module is currently active
     * @param moduleName Name of the module to check
     * @return true if the module is active, false otherwise
     */
    bool isModuleActive(const std::string& moduleName) const;

    /**
     * @brief Get the last error message (if updateActiveModules failed)
     */
    std::string getLastError() const;

private:
    // Currently active module instances (ready for Engine)
    std::vector<std::shared_ptr<IModule>> activeModules_;

    // Metadata about active modules (for state tracking)
    std::vector<SelectedModule> activeModulesInfo_;

    // Last error message
    std::string lastError_;

    /**
     * @brief Validate parameters against module's parameter descriptors
     * @return true if valid, false otherwise (sets lastError_)
     */
    bool validateParameters(const ModuleInfo& moduleInfo, const ParamMap& params);

    /**
     * @brief Helper to create a single module instance
     * @return Shared pointer to module if successful, nullptr otherwise
     */
    std::shared_ptr<IModule> createModuleInstance(const SelectedModule& selected);
};


#endif // MODULEMANAGER_HPP
