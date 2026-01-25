#include "TextLowerFirstLetterModule.hpp"
#include <cctype>

std::string TextLowerFirstLetterModule::getName() const {
    return "Text Lower First Letter Module";
}

ModuleResult TextLowerFirstLetterModule::apply(IData& data) {
    ModuleResult res;

    auto* textData = dynamic_cast<TextData*>(&data);
    if (!textData) {
        res.addError(ErrorType::Error, "INVALID_TYPE", "TextData expected");
        return res;
    }

    std::string content = textData->getContent();
    bool newSentence = true;

    for (char& c : content) {
        if (newSentence && std::isalpha(static_cast<unsigned char>(c))) {
            c = std::tolower(c);
            newSentence = false;
        }
        if (c == '.' || c == '!' || c == '?')
            newSentence = true;
    }

    textData->setContent(content);
    res.set<std::string>("LowerFirstLetterText", content);
    res.message = "Lowercase first letter applied";

    return res;
}
