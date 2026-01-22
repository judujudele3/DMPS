#include "TextWordCountModule.hpp"
#include <cctype>

std::string WordCountModule::getName() const {
    return "Word Count Module";
}

ModuleResult WordCountModule::apply(IData& data) {
    ModuleResult res;

    if (data.type() != DataType::Text) {
        res.addError(ErrorType::Error,
                     "INVALID_DATA_TYPE",
                     "WordCountModule expects TextData.");
        return res;
    }

    TextData& text = dynamic_cast<TextData&>(data);
    const std::string& content = text.getContent();

    size_t words = 0;
    bool inWord = false;

    for (char c : content) {
        if (std::isspace(c)) {
            if (inWord) words++;
            inWord = false;
        } else {
            inWord = true;
        }
    }
    if (inWord) words++;

    res.message = "Word count = " + std::to_string(words);
    res.set("WordCount",words);
    return res;
}
