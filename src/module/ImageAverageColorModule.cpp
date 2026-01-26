#include "ImageAverageColorModule.hpp"
#include <numeric>
#include <omp.h> // OpenMP pour parallélisation

std::string ImageAverageColorModule::getName() const {
    return "ImageAverageColorModule";
}

ModuleResult ImageAverageColorModule::apply(IData& data) {
    ModuleResult result;

    if (data.type() != DataType::Image) {
        result.addError(ErrorType::Error, "INVALID_DATA_TYPE",
                        "ImageAverageColorModule expects ImageData.");
        return result;
    }

    auto* image = dynamic_cast<ImageData*>(&data);
    if (!image) {
        result.addError(ErrorType::Error, "IMG_INVALID_TYPE",
                        "ImageAverageColorModule expects ImageData.");
        return result;
    }

    auto& pixels = image->pixels();
    size_t n = pixels.size();

    uint64_t sum_r = 0, sum_g = 0, sum_b = 0;
    int width = image->getWidth();
    int height = image->getHeight();

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            Pixel& p = image->at(x, y);
            sum_r += p.r;
            sum_g += p.g;
            sum_b += p.b;
        }
    }

    uint8_t avg_r = static_cast<uint8_t>(sum_r / n);
    uint8_t avg_g = static_cast<uint8_t>(sum_g / n);
    uint8_t avg_b = static_cast<uint8_t>(sum_b / n);

    result.set("AvgR", avg_r);
    result.set("AvgG", avg_g);
    result.set("AvgB", avg_b);
    result.message = "Average color successfully computed.";

    return result;
}
