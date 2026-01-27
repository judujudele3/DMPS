#ifndef TABULARDESCRIBE_HPP
#define TABULARDESCRIBE_HPP

#include "IModule.hpp"
#include "../data/TabularData.hpp"

class TabularDescribe : public IModule {
public:
    std::string getName() const override;
    ModuleResult apply(IData& data) override;
    static std::vector<ModuleParameter> getParameterDescriptors();
};

#endif
