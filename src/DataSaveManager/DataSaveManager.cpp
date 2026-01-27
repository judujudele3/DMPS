#include "DataSaveManager.hpp"
#include <algorithm>

std::vector<std::string> DataSaveManager::availableFormats(const IData& data) const {
    std::vector<std::string> formats;
    for (auto& saver : savers) {
        auto exts = saver->supportedExtensions();
        formats.insert(formats.end(), exts.begin(), exts.end());
    }
    return formats;
}

bool DataSaveManager::saveData(const IData& data, const std::string& path) const {
    size_t pos = path.find_last_of('.');
    if (pos == std::string::npos) {
        std::cerr << "Error: No file extension provided.\n";
        return false;
    }
    std::string ext = path.substr(pos + 1);
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

    for (auto& saver : savers) {
        auto exts = saver->supportedExtensions();
        if (std::find(exts.begin(), exts.end(), ext) != exts.end()) {
            if (saver->save(data, path)) {
                std::cout << "Data saved successfully to " << path << "\n";
                return true;
            } else {
                std::cerr << "Error: Failed to save data to " << path << "\n";
                return false;
            }
        }
    }

    std::cerr << "Error: No saver supports the extension ." << ext << "\n";
    return false;
}
