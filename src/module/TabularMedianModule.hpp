#ifndef TABULARMEDIANMODULE_HPP
#define TABULARMEDIANMODULE_HPP

#include "IModule.hpp"
#include "../data/TabularData.hpp"
#include <string>

class TabularMedianModule : public IModule {
public:
    explicit TabularMedianModule(const std::string& colName);

    std::string getName() const override;
    ModuleResult apply(IData& data) override;
    static std::vector<ModuleParameter> getParameterDescriptors();

private:
    std::string m_colName;
};

#endif // TABULARMEDIANMODULE_HPP
