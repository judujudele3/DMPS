#include "TabularDataLoader.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>

bool TabularDataLoader::supports(const std::string& extension) const {
    std::string ext = extension;
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    return ext == "csv"; // pour l'instant, on supporte que CSV
}

std::string TabularDataLoader::getExtension(const std::string& path) const {
    size_t pos = path.find_last_of('.');
    if (pos == std::string::npos) return "";
    std::string ext = path.substr(pos + 1);
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    return ext;
}

std::shared_ptr<IData> TabularDataLoader::load(const std::string& path) {
    std::string ext = getExtension(path);
    if (!supports(ext)) {
        std::cerr << "Erreur: format non supporté pour " << path << std::endl;
        return nullptr;
    }

    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Erreur: impossible d'ouvrir le fichier " << path << std::endl;
        return nullptr;
    }

    auto tabData = std::make_shared<TabularData>(TableFormat::CSV);

    std::string line;
    bool firstLine = true;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string cell;
        std::vector<std::string> row;

        while (std::getline(ss, cell, ',')) {
            row.push_back(cell);
        }

        if (firstLine) {
            tabData->setHeader(row);
            firstLine = false;
        } else {
            tabData->addRow(row);
        }
    }

    file.close();
    return tabData;
}
