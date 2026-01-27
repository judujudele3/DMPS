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



ModuleResult Engine::applyModules() {
    ModuleResult finalResult;

    if (!m_data) {
        finalResult.addError(ErrorType::Error, "NO_DATA",
                             "No data was set in the engine.");
        return finalResult;
    }

    for (auto& module : m_modules) {
        ModuleResult result = module->apply(*m_data);

        // Ajoute toutes les issues au résultat global
        for (auto& issue : result.issues) {
            finalResult.issues.push_back(issue);
        }

        // Si le module donne un message principal, on peut le conserver
        if (!result.message.empty()) {
            finalResult.message += result.message + "\n";
        }
    }

    return finalResult;
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
