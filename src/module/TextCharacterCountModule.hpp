#ifndef TEXTCHARACTERCOUNTMODULE_HPP
#define TEXTCHARACTERCOUNTMODULE_HPP

#include "IModule.hpp"
#include "../data/TextData.hpp"

class TextCharacterCountModule : public IModule {
public:
    std::string getName() const override ;

    ModuleResult apply(IData& data) override;
    static std::vector<ModuleParameter> getParameterDescriptors();
};

#endif
