#include "ImageInvertColorsModule.hpp"

std::string ImageInvertColorsModule::getName() const {
    return "ImageInvertColorsModule";
}

ModuleResult ImageInvertColorsModule::apply(IData& data) {
    ModuleResult result;

    if (data.type() != DataType::Image) {
        result.addError(ErrorType::Error, "INVALID_DATA_TYPE",
                        "ImageInvertColorsModule expects ImageData.");
        return result;
    }

    auto* image = dynamic_cast<ImageData*>(&data);
    if (!image) {
        result.addError(ErrorType::Error, "IMG_INVALID_TYPE",
                        "ImageInvertColorsModule expects ImageData.");
        return result;
    }

    for (int y = 0; y < image->getHeight(); ++y) {
        for (int x = 0; x < image->getWidth(); ++x) {
            Pixel& p = image->at(x, y);
            p.r = 255 - p.r;
            p.g = 255 - p.g;
            p.b = 255 - p.b;
        }
    }

    result.message = "Image colors inverted.";
    return result;
}

std::vector<ModuleParameter>
ImageInvertColorsModule::getParameterDescriptors() {
    return {}; // Aucun paramètre
}
