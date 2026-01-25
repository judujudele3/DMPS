#ifndef TEXTLINECOUNTMODULE_HPP
#define TEXTLINECOUNTMODULE_HPP

#include "IModule.hpp"
#include "../data/TextData.hpp"

class TextLineCountModule : public IModule {
public:
    std::string getName() const override {
        return "Text Line Count";
    }

    ModuleResult apply(IData& data) override;
};

#endif
