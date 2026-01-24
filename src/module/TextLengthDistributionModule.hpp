#ifndef TEXTLENGTHDISTRIBUTIONMODULE_HPP
#define TEXTLENGTHDISTRIBUTIONMODULE_HPP

#include "IModule.hpp"
#include "../data/TextData.hpp"
#include <unordered_map>

class TextLengthDistributionModule : public IModule {
public:
    std::string getName() const override { return "Text Length Distribution"; }
    ModuleResult apply(IData& data) override;
};

#endif
