#ifndef TABULARISNULLMODULE_HPP
#define TABULARISNULLMODULE_HPP

#include "IModule.hpp"
#include "../data/TabularData.hpp"
#include <string>

class TabularIsNullModule : public IModule {
public:
    explicit TabularIsNullModule(const std::string& colName);

    std::string getName() const override;
    ModuleResult apply(IData& data) override;
    static std::vector<ModuleParameter> getParameterDescriptors();

private:
    std::string m_colName;
};

#endif // TABULARISNULLMODULE_HPP
