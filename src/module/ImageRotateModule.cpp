#include "ImageRotateModule.hpp"
#include <vector>

std::string ImageRotateModule::getName() const {
    return "ImageRotateModule";
}

ModuleResult ImageRotateModule::apply(IData& data) {
    ModuleResult result;
    if (data.type() != DataType::Image) {
        result.addError(ErrorType::Error, "INVALID_DATA_TYPE",
                        "ImageRotateModule expects ImageData.");
        return result;
    }

    auto* image = dynamic_cast<ImageData*>(&data);
    if (!image) return result;

    int w = image->getWidth();
    int h = image->getHeight();
    std::vector<Pixel> buffer = image->pixels();

    if (degree == RotateDegree::R180) {
        for (int y = 0; y < h; ++y) {
            for (int x = 0; x < w; ++x) {
                image->at(x, y) = buffer[(h - 1 - y) * w + (w - 1 - x)];
            }
        }
    } else if (degree == RotateDegree::R90) {
        std::vector<Pixel> tmp(h * w);
        for (int y = 0; y < h; ++y)
            for (int x = 0; x < w; ++x)
                tmp[x * h + (h - 1 - y)] = buffer[y * w + x];
        image->pixels() = tmp;
        std::swap(image->getWidth(), image->getHeight());
    } else if (degree == RotateDegree::R270) {
        std::vector<Pixel> tmp(h * w);
        for (int y = 0; y < h; ++y)
            for (int x = 0; x < w; ++x)
                tmp[(w - 1 - x) * h + y] = buffer[y * w + x];
        image->pixels() = tmp;
        std::swap(image->getWidth(), image->getHeight());
    }

    result.message = "Image rotated.";
    return result;
}
