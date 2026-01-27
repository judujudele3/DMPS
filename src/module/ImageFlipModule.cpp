#include "ImageFlipModule.hpp"

std::string ImageFlipModule::getName() const {
    return "ImageFlipModule";
}

ModuleResult ImageFlipModule::apply(IData& data) {
    ModuleResult result;

    if (data.type() != DataType::Image) {
        result.addError(ErrorType::Error, "INVALID_DATA_TYPE",
                        "ImageFlipModule expects ImageData.");
        return result;
    }

    auto* image = dynamic_cast<ImageData*>(&data);
    if (!image) return result;

    int w = image->getWidth();
    int h = image->getHeight();

    if (type == FlipType::Horizontal) {
        for (int y = 0; y < h; ++y) {
            for (int x = 0; x < w / 2; ++x) {
                std::swap(image->at(x, y), image->at(w - 1 - x, y));
            }
        }
    } else { // Vertical
        for (int y = 0; y < h / 2; ++y) {
            for (int x = 0; x < w; ++x) {
                std::swap(image->at(x, y), image->at(x, h - 1 - y));
            }
        }
    }

    result.message = "Image flipped.";
    return result;
}

std::vector<ModuleParameter>
ImageFlipModule::getParameterDescriptors() {
    return {
        {
            "type",
            "string",
            std::string("Horizontal"),
            "Type de retournement : Horizontal ou Vertical"
        }
    };
}
