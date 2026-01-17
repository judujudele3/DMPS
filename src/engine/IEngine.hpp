#ifndef IENGINE_HPP
#define IENGINE_HPP
#pragma once
#include <memory>
#include "../core/ModuleResult.hpp"

class IData;
class IModule;

class IEngine {
public:
    virtual ~IEngine() = default;

    virtual void setData(std::shared_ptr<IData> data) = 0;
    virtual void addModule(std::shared_ptr<IModule> module) = 0;
    virtual ModuleResult applyModules() = 0;


    virtual std::shared_ptr<IData> getData() const = 0; // récupérer la donnée actuelle
};

#endif // IENGINE_HPP
