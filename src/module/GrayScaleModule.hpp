#ifndef GRAYSCALEMODULE_HPP
#define GRAYSCALEMODULE_HPP

#include "IModule.hpp"
#include "../data/ImageData.hpp"

class GrayScaleModule : public IModule {
public:
    GrayScaleModule() = default;

    std::string getName() const override;
    ModuleResult apply(IData& data) override;
};

#endif // GRAYSCALEMODULE_HPP
