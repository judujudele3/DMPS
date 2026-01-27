#ifndef TABULARMINMODULE_HPP
#define TABULARMINMODULE_HPP

#include "IModule.hpp"
#include "../data/TabularData.hpp"
#include <string>

class TabularMinModule : public IModule {
public:
    explicit TabularMinModule(const std::string& colName);

    std::string getName() const override;
    ModuleResult apply(IData& data) override;
    static std::vector<ModuleParameter> getParameterDescriptors();

private:
    std::string m_colName;
};

#endif // TABULARMINMODULE_HPP
