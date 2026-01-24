#ifndef TEXTTRIMLINESMODULE_HPP
#define TEXTTRIMLINESMODULE_HPP

#include "IModule.hpp"
#include "../data/TextData.hpp"

class TextTrimLinesModule : public IModule {
public:
    std::string getName() const override;
    ModuleResult apply(IData& data) override;
};

#endif
