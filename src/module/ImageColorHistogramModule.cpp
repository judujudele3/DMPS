#include "ImageColorHistogramModule.hpp"

std::string ImageColorHistogramModule::getName() const {
    return "ImageColorHistogramModule";
}

ModuleResult ImageColorHistogramModule::apply(IData& data) {
    ModuleResult result;

    // Vérification logique
    if (data.type() != DataType::Image) {
        result.addError(ErrorType::Error, "INVALID_DATA_TYPE",
                        "ImageColorHistogramModule expects ImageData.");
        return result;
    }

    // Vérification réelle
    auto* image = dynamic_cast<ImageData*>(&data);
    if (!image) {
        result.addError(ErrorType::Error, "IMG_INVALID_TYPE",
                        "ImageColorHistogramModule expects ImageData.");
        return result;
    }

    // Initialisation des histogrammes
    std::vector<size_t> histR(256, 0);
    std::vector<size_t> histG(256, 0);
    std::vector<size_t> histB(256, 0);

    // 🔹 Parcours avec at(x,y)
    int width = image->getWidth();
    int height = image->getHeight();

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            const Pixel& p = image->at(x, y);
            ++histR[p.r];
            ++histG[p.g];
            ++histB[p.b];
        }
    }

    // Stockage des résultats
    result.set("HistR", histR);
    result.set("HistG", histG);
    result.set("HistB", histB);

    result.message = "Color histogram successfully computed.";
    return result;
}

std::vector<ModuleParameter>
ImageColorHistogramModule::getParameterDescriptors() {
    return {};
}

