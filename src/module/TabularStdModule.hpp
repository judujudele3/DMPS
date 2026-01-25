#ifndef TABULARSTDMODULE_HPP
#define TABULARSTDMODULE_HPP

#include "IModule.hpp"
#include "../data/TabularData.hpp"
#include <string>
#include<cmath>

class TabularStdModule : public IModule {
public:
    explicit TabularStdModule(const std::string& colName);

    std::string getName() const override;
    ModuleResult apply(IData& data) override;

private:
    std::string m_colName;
};

#endif // TABULARSTDMODULE_HPP
