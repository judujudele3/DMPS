
#include "TextDataLoader.hpp"
#include "../data/TextData.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include<string>
#include<set>

bool TextDataLoader::supports(const std::string& extension) const
{
    static const std::set<std::string> supportedExtensions = {
        ".txt", ".md", ".log",
        ".cpp", ".hpp", ".h", ".c",
        ".java", ".py", ".js", ".html", ".css",
        ".json", ".xml", ".yaml", ".yml",
        ".ini", ".cfg", ".conf"
    };

    return supportedExtensions.find(extension) != supportedExtensions.end();
}

std::string TextDataLoader::getExtension(const std::string& path) const {
    size_t pos = path.find_last_of('.');
    if (pos == std::string::npos) return "";
    std::string ext = path.substr(pos);
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    return ext;
}

std::shared_ptr<IData> TextDataLoader::load(const std::string& path)
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        std::cerr << "Unable to open the file : " << path << std::endl;
        return nullptr;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();

    std::cout << "[TextDataLoader] File loaded "<< std::endl;

    // Adding a default format
    return std::make_shared<TextData>(content);
}
