#ifndef TABULARNORMALIZEMODULE_HPP
#define TABULARNORMALIZEMODULE_HPP

#include "IModule.hpp"
#include "../data/TabularData.hpp"

class TabularNormalizeModule : public IModule {
public:
    std::string getName() const override;
    ModuleResult apply(IData& data) override;
    static std::vector<ModuleParameter> getParameterDescriptors();
};

#endif
