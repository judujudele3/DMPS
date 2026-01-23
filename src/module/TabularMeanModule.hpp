#ifndef TABULARMEANMODULE_HPP
#define TABULARMEANMODULE_HPP

#include "IModule.hpp"
#include "../data/TabularData.hpp"
#include <string>

class TabularMeanModule : public IModule {
public:
    explicit TabularMeanModule(const std::string& colName);

    std::string getName() const override;
    ModuleResult apply(IData& data) override;

private:
    std::string m_colName;
};

#endif // TABULARMEANMODULE_HPP
