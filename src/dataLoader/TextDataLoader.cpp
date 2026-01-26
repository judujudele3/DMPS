#include "TextDataLoader.hpp"
#include "../data/TextData.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>


bool TextDataLoader::supports(const std::string& extension) const
{
    return extension == ".txt";
}

std::string TextDataLoader::getExtension(const std::string& path) const {
    size_t pos = path.find_last_of('.');
    if (pos == std::string::npos) return "";
    std::string ext = path.substr(pos + 1);
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    return ext;
}

std::shared_ptr<IData> TextDataLoader::load(const std::string& path)
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        std::cerr << "[TextDataLoader] Impossible d'ouvrir le fichier : " << path << std::endl;
        return nullptr;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();

    return std::make_shared<TextData>(content);
}
