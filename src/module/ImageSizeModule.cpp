#include "ImageSizeModule.hpp"

std::string ImageSizeModule::getName() const {
    return "ImageSizeModule";
}

ModuleResult ImageSizeModule::apply(IData& data) {
    ModuleResult result;

    if (data.type() != DataType::Image) {
        result.addError(ErrorType::Error, "INVALID_DATA_TYPE",
                        "ImageSizeModule expects ImageData.");
        return result;
    }

    auto* image = dynamic_cast<ImageData*>(&data);
    if (!image) {
        result.addError(ErrorType::Error, "IMG_INVALID_TYPE",
                        "ImageSizeModule expects ImageData.");
        return result;
    }

    int width = image->getWidth();
    int height = image->getHeight();
    int total_pixels = width * height;

    result.set("Width", width);
    result.set("Height", height);
    result.set("TotalPixels", total_pixels);
    result.message = "Image size successfully computed.";

    return result;
}
