#pragma once
#include "IDataSaver.hpp"
#include "../data/TabularData.hpp"
#include <fstream>
#include <iostream>

class TabularDataSaver : public IDataSaver {
public:
    std::vector<std::string> supportedExtensions() const override {
        return { "csv", "xlsx" };
    }

    bool save(const IData& data, const std::string& path) override;
};
