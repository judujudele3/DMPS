#ifndef IMAGESIZEMODULE_HPP
#define IMAGESIZEMODULE_HPP

#include "IModule.hpp"
#include "../data/ImageData.hpp"

class ImageSizeModule : public IModule {
public:
    std::string getName() const override;
    ModuleResult apply(IData& data) override;
    static std::vector<ModuleParameter> getParameterDescriptors();

};

#endif
