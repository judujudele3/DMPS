#include "TextLanguageDetectionModule.hpp"


std::string TextLanguageDetectionModule::getName() const {
    return "Language Detection Module";
}

ModuleResult TextLanguageDetectionModule::apply(IData& data) {
    ModuleResult res;
    auto* textData = dynamic_cast<TextData*>(&data);
    if (!textData) {
        res.addError(ErrorType::Error,"INVALID_TYPE","TextData expected");
        return res;
    }

    std::string content = textData->getContent();
    bool hasFrenchAccent = content.find_first_of("éèàùâêîôûç") != std::string::npos;
    std::string language = hasFrenchAccent ? "French" : "English";

    res.set<std::string>("Language", language);
    res.message = "Language detected successfully";
    return res;
}
