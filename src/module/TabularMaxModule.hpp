#ifndef TABULARMAXMODULE_HPP
#define TABULARMAXMODULE_HPP

#include "IModule.hpp"
#include "../data/TabularData.hpp"
#include <string>

class TabularMaxModule : public IModule {
public:
    explicit TabularMaxModule(const std::string& colName);

    std::string getName() const override;
    ModuleResult apply(IData& data) override;

private:
    std::string m_colName;
};

#endif // TABULARMAXMODULE_HPP
