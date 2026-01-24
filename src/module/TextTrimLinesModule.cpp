#include "TextTrimLinesModule.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>

static inline void trim(std::string& s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(),
        [](unsigned char ch) { return !std::isspace(ch); }));

    s.erase(std::find_if(s.rbegin(), s.rend(),
        [](unsigned char ch) { return !std::isspace(ch); }).base(), s.end());
}

std::string TextTrimLinesModule::getName() const {
    return "Text Trim Lines Module";
}

ModuleResult TextTrimLinesModule::apply(IData& data) {
    ModuleResult res;

    auto* textData = dynamic_cast<TextData*>(&data);
    if (!textData) {
        res.addError(ErrorType::Error, "INVALID_TYPE", "TextData expected");
        return res;
    }

    std::istringstream iss(textData->getContent());
    std::string line, result;

    while (std::getline(iss, line)) {
        trim(line);
        result += line + "\n";
    }

    textData->setContent(result);
    res.set<std::string>("TrimmedText", result);
    res.message = "Lines trimmed";

    return res;
}
