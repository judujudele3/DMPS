#ifndef IMAGEBRIGHTNESSMODULE_HPP
#define IMAGEBRIGHTNESSMODULE_HPP

#include "IModule.hpp"
#include "../data/ImageData.hpp"


class ImageBrightnessModule : public IModule {
public:
    std::string getName() const override;
    ModuleResult apply(IData& data) override;
    static std::vector<ModuleParameter> getParameterDescriptors();
private:
    // Calcul de la luminosité perceptuelle d’un pixel
    uint8_t intensity(const Pixel& p) const {
        return static_cast<uint8_t>(0.299 * p.r + 0.587 * p.g + 0.114 * p.b);
    }
};

#endif
