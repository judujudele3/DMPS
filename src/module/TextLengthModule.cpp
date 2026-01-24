#include "TextLengthModule.hpp"

std::string TextLengthModule::getName() const {
    return "Text Length Module";
}

ModuleResult TextLengthModule::apply(IData& data) {
    ModuleResult res;

    if (data.type() != DataType::Text) {
        res.addError(ErrorType::Error,
                     "INVALID_DATA_TYPE",
                     "TextLengthModule expected TextData but received another type.");
        return res;
    }

    TextData& text = dynamic_cast<TextData&>(data);

    // Valeur d'information : longueur du texte
    res.set("ContentLength", static_cast<int>(text.getContent().size()));
    res.message = "Content length = " + std::to_string(text.getContent().size()) + " characters.";

    return res;
}
