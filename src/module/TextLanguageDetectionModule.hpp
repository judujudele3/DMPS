#ifndef TEXTLANGUAGEDETECTIONMODULE_HPP
#define TEXTLANGUAGEDETECTIONMODULE_HPP

#include "IModule.hpp"
#include "../data/TextData.hpp"
#include <string>

class TextLanguageDetectionModule : public IModule {
public:
    std::string getName() const override;
    ModuleResult apply(IData& data) override;
};

#endif
