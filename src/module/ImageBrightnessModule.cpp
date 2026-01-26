#include "ImageBrightnessModule.hpp"
#include <cstdint>
std::string ImageBrightnessModule::getName() const {
    return "ImageBrightnessModule";
}

ModuleResult ImageBrightnessModule::apply(IData& data) {
    ModuleResult result;

    // Vérification du type
    if (data.type() != DataType::Image) {
        result.addError(ErrorType::Error, "INVALID_DATA_TYPE",
                        "ImageBrightnessModule expects ImageData.");
        return result;
    }

    auto* image = dynamic_cast<ImageData*>(&data);
    if (!image) {
        result.addError(ErrorType::Error, "IMG_INVALID_TYPE",
                        "ImageBrightnessModule expects ImageData.");
        return result;
    }

    int width = image->getWidth();
    int height = image->getHeight();
    double sum = 0;
    int minB = 255, maxB = 0;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            const Pixel& p = image->at(x, y);
            int i = intensity(p);
            sum += i;
            if (i < minB) minB = i;
            if (i > maxB) maxB = i;
        }
    }

    double avg = static_cast<double>(sum / (width * height));

    result.set("AvgBrightness", avg);
    result.set("MinBrightness",minB);
    result.set("MaxBrightness", maxB);
    result.message = "Image brightness successfully computed.";

    return result;
}
