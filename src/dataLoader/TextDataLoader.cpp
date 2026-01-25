#include "TextDataLoader.hpp"
#include "../data/TextData.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

bool TextDataLoader::supports(const std::string& extension) const
{
    return extension == ".txt";
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
