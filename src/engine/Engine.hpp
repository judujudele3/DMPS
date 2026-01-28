#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "IEngine.hpp"
#include "../dataLoaderManager/DataLoaderManager.hpp"
#include "../moduleManager/ModuleManager.hpp"
#include "../DataSaveManager/DataSaveManager.hpp"
#include <chrono>
#include <vector>
#include <memory>

class Engine : public IEngine {
public:
    Engine();
    ~Engine() override = default;

    void setData(std::shared_ptr<IData> data) override;
    std::vector<ModuleExecutionResult> applyModules() override;
    std::shared_ptr<IData> getData() const override;
    std::shared_ptr<IData> loadData(const std::string& path);
    void clearData();
    bool setActiveModules(const std::vector<SelectedModule>& selectedModules);
    std::vector<SelectedModule> getActiveModulesInfo() const;
    void clearModules();
    size_t getModuleCount() const;
    std::string getLastModuleError() const;
    bool saveData(const std::string& path) const;
    std::vector<std::string> getAvailableSaveFormats() const;
    DataSaveManager& getSaveManager() const;


private:
    std::shared_ptr<IData> m_data;
    std::vector<std::shared_ptr<IModule>> m_modules;
    std::unique_ptr<DataLoaderManager> m_loaderManager;
    std::unique_ptr<DataSaveManager> m_saveManager;
    std::unique_ptr<ModuleManager> m_moduleManager;
};

#endif // ENGINE_HPP
