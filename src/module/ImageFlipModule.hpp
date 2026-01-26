#ifndef IMAGEFLIPMODULE_HPP
#define IMAGEFLIPMODULE_HPP

#include "IModule.hpp"
#include "../data/ImageData.hpp"

enum class FlipType { Horizontal, Vertical };

class ImageFlipModule : public IModule {
public:
    ImageFlipModule(FlipType t) : type(t) {}
    std::string getName() const override;
    ModuleResult apply(IData& data) override;

private:
    FlipType type;
};

#endif
