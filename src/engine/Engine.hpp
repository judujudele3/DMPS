#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "IEngine.hpp"
#include "../core/ModuleResult.hpp"
#include <vector>
#include <memory>

class Engine : public IEngine {
public:
    Engine() = default;
    ~Engine() override = default;

    void setData(std::shared_ptr<IData> data) override;
    void addModule(std::shared_ptr<IModule> module) override;
    ModuleResult applyModules() override;
    std::shared_ptr<IData> getData() const override;

private:
    std::shared_ptr<IData> m_data;
    std::vector<std::shared_ptr<IModule>> m_modules;
};

#endif // ENGINE_HPP
