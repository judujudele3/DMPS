#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "IEngine.hpp"
#include "../core/ModuleResult.hpp"
#include "../dataLoaderManager/DataLoaderManager.hpp"
#include <vector>
#include <memory>

class Engine : public IEngine {
public:
    Engine();
    ~Engine() override = default;

    void setData(std::shared_ptr<IData> data) override;
    void addModule(std::shared_ptr<IModule> module) override;
    ModuleResult applyModules() override;
    std::shared_ptr<IData> getData() const override;
    std::shared_ptr<IData> loadData(const std::string& path);

private:
    DataLoaderManager m_loaderManager;
    std::shared_ptr<IData> m_data;
    std::vector<std::shared_ptr<IModule>> m_modules;
};

#endif // ENGINE_HPP
