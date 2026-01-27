#pragma once
#include "../include/stb_image_write.h"
#include "IDataSaver.hpp"
#include "../data/ImageData.hpp"
#include <iostream>
#include <string>
#include <fstream>


class ImageDataSaver : public IDataSaver {
public:
    std::vector<std::string> supportedExtensions() const override {
        return { "png", "jpg", "bmp" };
    }

    bool save(const IData& data, const std::string& path) override;
};
