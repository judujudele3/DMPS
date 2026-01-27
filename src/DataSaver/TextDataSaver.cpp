#include "TextDataSaver.hpp"

bool TextDataSaver::save(const IData& data, const std::string& path) {
    const TextData* textData = dynamic_cast<const TextData*>(&data);
    if (!textData) {
        std::cerr << "Error: Provided data is not TextData.\n";
        return false;
    }

    std::ofstream out(path);
    if (!out) {
        std::cerr << "Error: Cannot open file " << path << " for writing.\n";
        return false;
    }

    out << textData->getContent(); // Assuming TextData has getText()
    out.close();
    return true;
}
