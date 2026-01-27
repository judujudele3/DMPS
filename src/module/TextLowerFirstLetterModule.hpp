#ifndef TEXTLOWERFIRSTLETTERMODULE_HPP
#define TEXTLOWERFIRSTLETTERMODULE_HPP

#include "IModule.hpp"
#include "../data/TextData.hpp"

class TextLowerFirstLetterModule : public IModule {
public:
    std::string getName() const override;
    ModuleResult apply(IData& data) override;
    static std::vector<ModuleParameter> getParameterDescriptors();
};

#endif
