#include "Engine.hpp"
#include "../module/IModule.hpp"
#include "../dataLoader/TextDataLoader.hpp"

Engine::Engine()
    : m_loaderManager(std::make_unique<DataLoaderManager>())
    , m_moduleManager(std::make_unique<ModuleManager>())
{
    // Enregistrer les loaders disponibles
    m_loaderManager->addLoader(std::make_shared<TextDataLoader>());
    // ... autres loaders
}

void Engine::setData(std::shared_ptr<IData> data) {
    m_data = data;
}

bool Engine::setActiveModules(const std::vector<SelectedModule>& selectedModules)
{
    // Delegate to ModuleManager to create instances
    bool success = m_moduleManager->updateActiveModules(selectedModules);

    if (!success) {
        return false;
    }

    // Update our module list with the newly created instances
    m_modules = m_moduleManager->getActiveModuleInstances();

    return true;
}

std::vector<SelectedModule> Engine::getActiveModulesInfo() const
{
    return m_moduleManager->getActiveModulesInfo();
}

void Engine::clearModules()
{
    m_modules.clear();
    m_moduleManager->clearActiveModules();
}

size_t Engine::getModuleCount() const
{
    return m_modules.size();
}

std::string Engine::getLastModuleError() const
{
    return m_moduleManager->getLastError();
}



std::vector<ModuleExecutionResult> Engine::applyModules()
{
    std::vector<ModuleExecutionResult> results;

    if (!m_data) {
        std::cerr << "[Engine] No data loaded. Cannot apply modules." << std::endl;
        return results;
    }

    if (m_modules.empty()) {
        std::cerr << "[Engine] No modules active. Nothing to apply." << std::endl;
        return results;
    }

    std::cout << "[Engine] Applying " << m_modules.size() << " module(s)..." << std::endl;

    // Apply each module sequentially (in-place modification)
    for (const auto& module : m_modules) {
        ModuleExecutionResult execResult;

        // Get module name
        execResult.moduleName = module->getName();

        // Try to get display name from registry
        const ModuleInfo* info = ModuleRegistry::findModule(execResult.moduleName);
        if (info) {
            execResult.displayName = info->displayName;
        } else {
            execResult.displayName = execResult.moduleName;
        }

        // Measure execution time
        auto startTime = std::chrono::high_resolution_clock::now();

        try {
            // Apply the module (modifies m_data in-place)
            execResult.result = module->apply(*m_data);

        } catch (const std::exception& e) {
            // Catch exceptions and convert to error
            execResult.result.success = false;
            execResult.result.addError(
                ErrorType::Critical,
                "EXCEPTION",
                std::string("Exception during module execution: ") + e.what()
                );
        } catch (...) {
            // Catch unknown exceptions
            execResult.result.success = false;
            execResult.result.addError(
                ErrorType::Critical,
                "UNKNOWN_EXCEPTION",
                "Unknown exception during module execution"
                );
        }

        auto endTime = std::chrono::high_resolution_clock::now();
        execResult.duration = std::chrono::duration_cast<std::chrono::milliseconds>(
            endTime - startTime
            );

        // Log result
        if (execResult.result.success) {
            std::cout << "[Engine]   ✅ " << execResult.displayName
                      << " completed in " << execResult.duration.count() << "ms" << std::endl;
        } else {
            std::cerr << "[Engine]   ❌ " << execResult.displayName
                      << " failed" << std::endl;
        }

        // Store result (continue even if failed)
        results.push_back(execResult);
    }

    std::cout << "[Engine] Module execution complete." << std::endl;

    return results;
}

std::shared_ptr<IData> Engine::getData() const {
    return m_data;
}

std::shared_ptr<IData> Engine::loadData(const std::string& path)
{
    auto data = m_loaderManager->loadData(path);
    if (data)
    {
        m_data = data;
        std::cout << "[Engine] Données chargées avec succès." << std::endl;
    }
    return data;
}
