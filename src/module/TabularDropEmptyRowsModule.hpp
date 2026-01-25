#ifndef TABULARDROPEMPTYROWSMODULE_HPP
#define TABULARDROPEMPTYROWSMODULE_HPP



#include "IModule.hpp"
#include "../data/tabulardata.hpp"
#include <algorithm>
#include <string>
#include <vector>

class TabularDropEmptyRowsModule : public IModule {
public:
    std::string getName() const override;
    ModuleResult apply(IData& data) override;
};

#endif
