#ifndef TABULARFILTERNUMERICMODULE_HPP
#define TABULARFILTERNUMERICMODULE_HPP

#include "IModule.hpp"
#include "../data/tabulardata.hpp"

class TabularFilterNumericColumnsModule : public IModule {
public:
    std::string getName() const override;
    ModuleResult apply(IData& data) override;
};

#endif
