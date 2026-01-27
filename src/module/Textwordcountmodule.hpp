#ifndef TEXTWORDCOUNTMODULE_H
#define TEXTWORDCOUNTMODULE_H

#include <string>
#include "IModule.hpp"
#include "../data/IData.hpp"
#include "../data/TextData.hpp"

class TextWordCountModule : public IModule {
public:
    std::string getName() const override;
    ModuleResult apply(IData& data) override;
    static std::vector<ModuleParameter> getParameterDescriptors();
};

#endif
