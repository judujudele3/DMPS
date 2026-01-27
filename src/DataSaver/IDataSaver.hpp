#pragma once
#include <string>
#include <vector>
#include "../data/IData.hpp"

class IDataSaver {
public:
    virtual ~IDataSaver() = default;

    // Returns the list of supported extensions (without dot)
    virtual std::vector<std::string> supportedExtensions() const = 0;

    // Save the data to the given path
    virtual bool save(const IData& data, const std::string& path) = 0;
};
