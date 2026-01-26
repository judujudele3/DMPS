#ifndef IMAGEAVERAGECOLORMODULE_HPP
#define IMAGEAVERAGECOLORMODULE_HPP


#include "IModule.hpp"
#include "../data/ImageData.hpp"

class ImageAverageColorModule : public IModule {
public:
    std::string getName() const override;
    ModuleResult apply(IData& data) override;
};

#endif
