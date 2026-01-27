#pragma once
#include "IDataSaver.hpp"
#include "../data/TextData.hpp"
#include <fstream>
#include <iostream>

class TextDataSaver : public IDataSaver {
public:
    std::vector<std::string> supportedExtensions() const override {
        return { "txt", "json" };
    }

    bool save(const IData& data, const std::string& path) override;
};
