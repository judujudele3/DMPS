#ifndef TEXTTOKENIZEMODULE_HPP
#define TEXTTOKENIZEMODULE_HPP

#include "IModule.hpp"
#include "../data/TextData.hpp"
#include <vector>
#include <string>

class TextTokenizeModule : public IModule {
public:
    std::string getName() const override { return "Text Tokenize Module"; }
    ModuleResult apply(IData& data) override;
};

#endif
