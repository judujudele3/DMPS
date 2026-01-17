#include "Engine.hpp"
#include "../module/IModule.hpp"


void Engine::setData(std::shared_ptr<IData> data) {
    m_data = data;
}

void Engine::addModule(std::shared_ptr<IModule> module) {
    m_modules.push_back(module);
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
