#include "TextLineCountModule.hpp"
#include <algorithm>

ModuleResult TextLineCountModule::apply(IData& data) {
    ModuleResult result;

    // Vérification du type
    auto* textData = dynamic_cast<TextData*>(&data);
    if (!textData) {
        result.addError(
            ErrorType::Error,
            "INVALID_DATA_TYPE",
            "TextLineCountModule expects TextData"
            );
        return result;
    }

    const std::string& content = textData->getContent();

    if (content.empty()) {
        result.set<int>("line_count", 0);
        result.message = "Empty text";
        return result;
    }

    int lineCount = 1 + std::count(content.begin(), content.end(), '\n');

    result.set("LineCount", lineCount);
    result.message = "Line count computed successfully";

    return result;
}

