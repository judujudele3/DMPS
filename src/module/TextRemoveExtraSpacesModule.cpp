#include "TextRemoveExtraSpacesModule.hpp"
#include <sstream>

std::string TextRemoveExtraSpacesModule::getName() const {
    return "Text Remove Extra Spaces Module";
}

ModuleResult TextRemoveExtraSpacesModule::apply(IData& data) {
    ModuleResult res;

    auto* textData = dynamic_cast<TextData*>(&data);
    if (!textData) {
        res.addError(ErrorType::Error, "INVALID_TYPE", "TextData expected");
        return res;
    }

    std::istringstream iss(textData->getContent());
    std::string word, cleaned;

    while (iss >> word) {
        if (!cleaned.empty())
            cleaned += " ";
        cleaned += word;
    }

    textData->setContent(cleaned);
    res.set<std::string>("CleanedText", cleaned);
    res.message = "Extra spaces removed";

    return res;
}
