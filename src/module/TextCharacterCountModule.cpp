#include "TextCharacterCountModule.hpp"


std::string TextCharacterCountModule::getName() const  {
    return "Text Character Count";
}


ModuleResult TextCharacterCountModule::apply(IData& data) {
    ModuleResult result;

    auto* textData = dynamic_cast<TextData*>(&data);
    if (!textData) {
        result.addError(
            ErrorType::Error,
            "INVALID_DATA_TYPE",
            "TextCharacterCountModule expects TextData"
            );
        return result;
    }

    const std::string& content = textData->getContent();

    int charCount = static_cast<int>(content.size());

    result.set("ContentLength", static_cast<int>(textData->getContent().size()));
    result.set("CharacterCount", charCount);
    result.message = "Character count computed successfully";

    return result;
}


std::vector<ModuleParameter> TextCharacterCountModule::getParameterDescriptors() {
    return {};
}
