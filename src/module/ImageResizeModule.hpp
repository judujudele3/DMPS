#ifndef IMAGERESIZEMODULE_HPP
#define IMAGERESIZEMODULE_HPP

#include "IModule.hpp"
#include "../data/ImageData.hpp"

class ImageResizeModule : public IModule {
public:
    ImageResizeModule(int newW, int newH) : width(newW), height(newH) {}
    std::string getName() const override;
    ModuleResult apply(IData& data) override;
    static std::vector<ModuleParameter> getParameterDescriptors();


private:
    int width, height;
};

#endif
