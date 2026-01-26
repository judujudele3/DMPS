#include "TextTokenizeModule.hpp"
#include <sstream>

std::string TextTokenizeModule::getName() const { return "Text Tokenize Module"; }



ModuleResult TextTokenizeModule::apply(IData& data) {
    ModuleResult res;

    auto* textData = dynamic_cast<TextData*>(&data);
    if (!textData) {
        res.addError(ErrorType::Error, "INVALID_TYPE", "TextData expected");
        return res;
    }

    std::vector<std::string> tokens;
    std::istringstream iss(textData->getContent());
    std::string word;
    while (iss >> word) {
        tokens.push_back(word);
    }

    res.set<std::vector<std::string>>("Tokens", tokens);
    res.message = "Text tokenized successfully";
    return res;
}
