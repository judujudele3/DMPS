#ifndef TEXTLINECOUNTMODULE_HPP
#define TEXTLINECOUNTMODULE_HPP

#include "IModule.hpp"
#include "../data/TextData.hpp"

class TextLineCountModule : public IModule {
public:
    std::string getName() const override;
    ModuleResult apply(IData& data) override;
    static std::vector<ModuleParameter> getParameterDescriptors();
};

#endif
