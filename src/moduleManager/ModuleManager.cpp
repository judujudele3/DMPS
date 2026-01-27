#include "ModuleManager.hpp"
#include <algorithm>

ModuleManager::ModuleManager()
    : lastError_("")
{
}

bool ModuleManager::updateActiveModules(const std::vector<SelectedModule>& selectedModules) {
    // Clear previous state
    activeModules_.clear();
    activeModulesInfo_.clear();
    lastError_.clear();

    // Create instances for each selected module
    for (const auto& selected : selectedModules) {
        auto instance = createModuleInstance(selected);

        if (!instance) {
            // Creation failed - lastError_ is already set by createModuleInstance
            activeModules_.clear();
            activeModulesInfo_.clear();
            return false;
        }

        activeModules_.push_back(instance);
        activeModulesInfo_.push_back(selected);
    }

    return true;
}

const std::vector<std::shared_ptr<IModule>>& ModuleManager::getActiveModuleInstances() const {
    return activeModules_;
}

std::vector<SelectedModule> ModuleManager::getActiveModulesInfo() const {
    return activeModulesInfo_;
}

void ModuleManager::clearActiveModules() {
    activeModules_.clear();
    activeModulesInfo_.clear();
    lastError_.clear();
}

size_t ModuleManager::getActiveModuleCount() const {
    return activeModules_.size();
}

bool ModuleManager::isModuleActive(const std::string& moduleName) const {
    return std::find_if(activeModulesInfo_.begin(), activeModulesInfo_.end(),
                        [&moduleName](const SelectedModule& sel) {
                            return sel.moduleName == moduleName;
                        }) != activeModulesInfo_.end();
}

std::string ModuleManager::getLastError() const {
    return lastError_;
}

bool ModuleManager::validateParameters(const ModuleInfo& moduleInfo, const ParamMap& params) {
    // Check that all required parameters are present
    for (const auto& paramDesc : moduleInfo.parameters) {
        auto it = params.find(paramDesc.name);

        if (it == params.end()) {
            lastError_ = "Missing parameter '" + paramDesc.name + "' for module '" + moduleInfo.name + "'";
            return false;
        }

        // Type checking
        const auto& value = it->second;
        bool typeMatch = false;

        if (paramDesc.type == "int" && std::holds_alternative<int>(value)) {
            typeMatch = true;
        } else if (paramDesc.type == "float" && std::holds_alternative<float>(value)) {
            typeMatch = true;
        } else if (paramDesc.type == "string" && std::holds_alternative<std::string>(value)) {
            typeMatch = true;
        } else if (paramDesc.type == "bool" && std::holds_alternative<bool>(value)) {
            typeMatch = true;
        }

        if (!typeMatch) {
            lastError_ = "Type mismatch for parameter '" + paramDesc.name +
                         "' in module '" + moduleInfo.name + "' (expected " + paramDesc.type + ")";
            return false;
        }

        // Constraint validation
        if (!paramDesc.isValid(value)) {
            lastError_ = "Parameter '" + paramDesc.name + "' in module '" + moduleInfo.name +
                         "' is out of bounds";
            return false;
        }
    }

    // Check for unexpected parameters (optional warning, not an error)
    for (const auto& [paramName, paramValue] : params) {
        auto it = std::find_if(moduleInfo.parameters.begin(), moduleInfo.parameters.end(),
                               [&paramName](const ModuleParameter& desc) {
                                   return desc.name == paramName;
                               });
        if (it == moduleInfo.parameters.end()) {
            // Could log a warning here
            // For now, we ignore unexpected parameters
        }
    }

    return true;
}

std::shared_ptr<IModule> ModuleManager::createModuleInstance(const SelectedModule& selected) {
    // Find module in registry
    const ModuleInfo* moduleInfo = ModuleRegistry::findModule(selected.moduleName);

    if (!moduleInfo) {
        lastError_ = "Module '" + selected.moduleName + "' not found in registry";
        return nullptr;
    }

    // Validate parameters
    if (!validateParameters(*moduleInfo, selected.parameters)) {
        // lastError_ already set by validateParameters
        return nullptr;
    }

    // Create instance via factory
    try {
        auto instance = moduleInfo->factory(selected.parameters);

        if (!instance) {
            lastError_ = "Factory for module '" + selected.moduleName + "' returned null";
            return nullptr;
        }

        return instance;
    }
    catch (const std::exception& e) {
        lastError_ = "Exception while creating module '" + selected.moduleName + "': " + e.what();
        return nullptr;
    }
    catch (...) {
        lastError_ = "Unknown exception while creating module '" + selected.moduleName + "'";
        return nullptr;
    }
}
