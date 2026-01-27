#include "ImageResizeModule.hpp"
#include <vector>

std::string ImageResizeModule::getName() const {
    return "ImageResizeModule";
}

ModuleResult ImageResizeModule::apply(IData& data) {
    ModuleResult result;
    if (data.type() != DataType::Image) {
        result.addError(ErrorType::Error, "INVALID_DATA_TYPE",
                        "ImageResizeModule expects ImageData.");
        return result;
    }

    auto* image = dynamic_cast<ImageData*>(&data);
    if (!image) return result;

    int oldW = image->getWidth();
    int oldH = image->getHeight();
    std::vector<Pixel> oldPixels = image->pixels();
    std::vector<Pixel> newPixels(width * height);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int srcX = x * oldW / width;
            int srcY = y * oldH / height;
            newPixels[y * width + x] = oldPixels[srcY * oldW + srcX];
        }
    }

    image->pixels() = newPixels;
    image->getWidth() = width;
    image->getHeight()= height;

    result.message = "Image resized.";
    return result;
}


std::vector<ModuleParameter>
ImageResizeModule::getParameterDescriptors() {
    return {
        {"width", "int", 256, "Nouvelle largeur de l'image", 1, 10000},
        {"height", "int", 256, "Nouvelle hauteur de l'image", 1, 10000}
    };
}
