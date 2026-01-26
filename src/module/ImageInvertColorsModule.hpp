#ifndef IMAGEINVERTCOLORSMODULE_HPP
#define IMAGEINVERTCOLORSMODULE_HPP

#include "IModule.hpp"
#include "../data/ImageData.hpp"

class ImageInvertColorsModule : public IModule {
public:
    std::string getName() const override;
    ModuleResult apply(IData& data) override;
};

#endif
