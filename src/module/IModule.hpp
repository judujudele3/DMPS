#ifndef IMODULE_HPP
#define IMODULE_HPP
#pragma once
#include <string>
#include "../core/ModuleResult.hpp"
#include "../core/ModuleParameter.hpp"

class IData; // forward declaration

class IModule {
public:
    virtual ~IModule() = default;
    virtual std::string getName() const = 0;
    virtual ModuleResult apply(IData& data) = 0;
};

#endif // IMODULE_HPP
