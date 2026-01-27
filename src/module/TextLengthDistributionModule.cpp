#include "TextLengthDistributionModule.hpp"
#include <sstream>


std::string TextLengthDistributionModule::getName() const  { return "Text Length Distribution"; }


ModuleResult TextLengthDistributionModule::apply(IData& data) {
    ModuleResult res;

    auto* textData = dynamic_cast<TextData*>(&data);
    if (!textData) {
        res.addError(ErrorType::Error, "INVALID_TYPE", "TextData expected");
        return res;
    }

    std::unordered_map<int,int> lengthHistogram; // longueur -> fréquence
    std::istringstream iss(textData->getContent());
    std::string word;
    while (iss >> word) {
        lengthHistogram[static_cast<int>(word.size())]++;
    }

    // Convertir en vecteur de chaînes pour ModuleResult
    std::vector<std::string> histogramStr;
    for (auto& [len, count] : lengthHistogram) {
        histogramStr.push_back("Length " + std::to_string(len) + ": " + std::to_string(count));
    }

    res.set<std::vector<std::string>>("LengthDistribution", histogramStr);
    res.message = "Word length distribution computed successfully";
    return res;
}


std::vector<ModuleParameter> TextLengthDistributionModule::getParameterDescriptors() {
    return {};
}
