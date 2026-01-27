#ifndef IMAGEGRAYSCALEMODULE_H
#define IMAGEGRAYSCALEMODULE_H

#include "IModule.hpp"
#include "../data/ImageData.hpp"

class ImageGrayScaleModule : public IModule {
public:
    ImageGrayScaleModule() = default;

    std::string getName() const override;
    ModuleResult apply(IData& data) override;
    static std::vector<ModuleParameter> getParameterDescriptors();

};

#endif // IMAGEGRAYSCALEMODULE_H
