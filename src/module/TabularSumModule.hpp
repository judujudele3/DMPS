#ifndef TABULARSUMMODULE_HPP
#define TABULARSUMMODULE_HPP

#include "IModule.hpp"
#include "../data/TabularData.hpp"
#include <string>

class TabularSumModule : public IModule {
public:
    explicit TabularSumModule(const std::string& columnName);

    std::string getName() const override;
    ModuleResult apply(IData& data) override;
    static std::vector<ModuleParameter> getParameterDescriptors();

private:
    std::string m_columnName;
};

#endif
