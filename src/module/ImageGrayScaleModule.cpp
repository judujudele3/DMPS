#include "ImageGrayScaleModule.hpp"

std::string ImageGrayScaleModule::getName() const {
    return "GrayScaleModule";
}

ModuleResult ImageGrayScaleModule::apply(IData& data) {
    ModuleResult result;

    // Vérification du type logique
    if (data.type() != DataType::Image) {
        result.addError(
            ErrorType::Error,
            "INVALID_DATA_TYPE",
            "GrayScaleModule expects ImageData."
            );
        return result;
    }


    // Vérifier que IData est bien une ImageData
    auto* image = dynamic_cast<ImageData*>(&data);
    if (!image) {
        result.addError(
            ErrorType::Error,
            "IMG_INVALID_TYPE",
            "GrayScaleModule expects ImageData"
            );
        return result;
    }

    // Conversion en niveaux de gris (in-place)
    for (int y = 0; y < image->getHeight(); ++y) {
        for (int x = 0; x < image->getWidth(); ++x) {
            Pixel& p = image->at(x, y);

            uint8_t gray =
                static_cast<uint8_t>((p.r + p.g + p.b) / 3);

            p.r = gray;
            p.g = gray;
            p.b = gray;
            // p.a reste inchangé
        }
    }

    result.message = "Image successfully converted to grayscale";
    return result;
}


std::vector<ModuleParameter>
ImageGrayScaleModule::getParameterDescriptors() {
    return {}; // Aucun paramètre
}
