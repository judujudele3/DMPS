#ifndef TABULARCOUNTMODULE_HPP
#define TABULARCOUNTMODULE_HPP

#include "IModule.hpp"
#include "../data/TabularData.hpp"
#include <string>

class TabularCountModule : public IModule {
public:
    explicit TabularCountModule(const std::string& colName);

    std::string getName() const override;
    ModuleResult apply(IData& data) override;

private:
    std::string m_colName;
};

#endif // TABULARCOUNTMODULE_HPP
