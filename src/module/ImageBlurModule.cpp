#include "ImageBlurModule.hpp"
#include <vector>

std::string ImageBlurModule::getName() const {
    return "ImageBlurModule";
}

ModuleResult ImageBlurModule::apply(IData& data) {
    ModuleResult result;

    if (data.type() != DataType::Image) {
        result.addError(ErrorType::Error, "INVALID_DATA_TYPE",
                        "ImageBlurModule expects ImageData.");
        return result;
    }

    auto* image = dynamic_cast<ImageData*>(&data);
    if (!image) return result;

    int w = image->getWidth();
    int h = image->getHeight();
    std::vector<Pixel> buffer = image->pixels(); // copie temporaire

    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            int sum_r = 0, sum_g = 0, sum_b = 0;
            int count = 0;

            // voisinage 3x3
            for (int dy = -1; dy <= 1; ++dy) {
                for (int dx = -1; dx <= 1; ++dx) {
                    int nx = x + dx;
                    int ny = y + dy;
                    if (nx >= 0 && nx < w && ny >= 0 && ny < h) {
                        const Pixel& p = image->at(nx, ny);
                        sum_r += p.r;
                        sum_g += p.g;
                        sum_b += p.b;
                        count++;
                    }
                }
            }

            Pixel& p = buffer[y * w + x];
            p.r = sum_r / count;
            p.g = sum_g / count;
            p.b = sum_b / count;
        }
    }

    image->pixels() = buffer; // copier le résultat
    result.message = "Image blurred.";
    return result;
}


std::vector<ModuleParameter>
ImageBlurModule::getParameterDescriptors() {
    return {}
