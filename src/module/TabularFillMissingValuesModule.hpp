#ifndef TABULARFILLMISSINGMODULE_HPP
#define TABULARFILLMISSINGMODULE_HPP

#include "IModule.hpp"
#include "../data/TabularData.hpp"

class TabularFillMissingModule : public IModule {
public:
    explicit TabularFillMissingModule(const std::string& column, const std::string& fillValue);
    std::string getName() const override;
    ModuleResult apply(IData& data) override;
    static std::vector<ModuleParameter> getParameterDescriptors();

private:
    std::string m_column;
    std::string m_fillValue;
};

#endif
