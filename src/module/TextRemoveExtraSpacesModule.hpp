#ifndef TEXTREMOVEEXTRASPACESMODULE_HPP
#define TEXTREMOVEEXTRASPACESMODULE_HPP

#include "IModule.hpp"
#include "../data/TextData.hpp"

class TextRemoveExtraSpacesModule : public IModule {
public:
    std::string getName() const override;
    ModuleResult apply(IData& data) override;
    static std::vector<ModuleParameter> getParameterDescriptors();
};

#endif
