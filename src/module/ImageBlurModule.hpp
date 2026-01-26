#ifndef IMAGEBLURMODULE_HPP
#define IMAGEBLURMODULE_HPP

#include "IModule.hpp"
#include "../data/ImageData.hpp"

class ImageBlurModule : public IModule {
public:
    std::string getName() const override;
    ModuleResult apply(IData& data) override;
};

#endif
