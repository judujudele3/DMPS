#ifndef TEXTLENGTHMODULE_HPP
#define TEXTLENGTHMODULE_HPP

#include "IModule.hpp"
#include "../data/IData.hpp"
#include "../data/TextData.hpp"
#include <string>
#include <memory>
#include "../core/ModuleParameter.hpp"

class TextLengthModule : public IModule {
public:
    TextLengthModule() = default;

    std::string getName() const override;

    ModuleResult apply(IData& data) override;
    static std::vector<ModuleParameter> getParameterDescriptors();
};

#endif
