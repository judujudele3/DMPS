#pragma once
#include <vector>
#include <memory>
#include <string>
#include <iostream>
#include "../DataSaver/IDataSaver.hpp"

class DataSaveManager {
public:
    void registerSaver(std::shared_ptr<IDataSaver> saver) {
        savers.push_back(saver);
    }

    // Returns all available extensions for the given data type
    std::vector<std::string> availableFormats(const IData& data) const;

    // Saves the data to the given path according to extension
    bool saveData(const IData& data, const std::string& path) const;

private:
    std::vector<std::shared_ptr<IDataSaver>> savers;
};
