#ifndef TABULARNORMALIZEMODULE_HPP
#define TABULARNORMALIZEMODULE_HPP

#include "IModule.hpp"
#include "../data/tabulardata.hpp"

class TabularNormalizeModule : public IModule {
public:
    std::string getName() const override;
    ModuleResult apply(IData& data) override;
};

#endif
