#ifndef TABULARSUMMARYMODULE_HPP
#define TABULARSUMMARYMODULE_HPP

#include "IModule.hpp"
#include "../data/tabulardata.hpp"

class TabularDescribe : public IModule {
public:
    std::string getName() const override;
    ModuleResult apply(IData& data) override;
};

#endif
