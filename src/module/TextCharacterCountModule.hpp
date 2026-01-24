#ifndef TEXTCHARACTERCOUNTMODULE_HPP
#define TEXTCHARACTERCOUNTMODULE_HPP

#include "IModule.hpp"
#include "../data/TextData.hpp"

class TextCharacterCountModule : public IModule {
public:
    std::string getName() const override {
        return "Text Character Count";
    }

    ModuleResult apply(IData& data) override;
};

#endif
