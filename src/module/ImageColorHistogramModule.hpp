#ifndef IMAGECOLORHISTOGRAMMODULE_HPP
#define IMAGECOLORHISTOGRAMMODULE_HPP

#include "IModule.hpp"
#include "../data/ImageData.hpp"

class ImageColorHistogramModule : public IModule {
public:
    std::string getName() const override;
    ModuleResult apply(IData& data) override;
    static std::vector<ModuleParameter> getParameterDescriptors();
};

#endif // IMAGECOLORHISTOGRAMMODULE_HPP
