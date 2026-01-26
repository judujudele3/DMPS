#include "ImageDataLoader.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"
#include <iostream>
#include <algorithm>

bool ImageDataLoader::supports(const std::string& extension) const {
    std::string ext = extension;
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    return ext == "jpg" || ext == "jpeg" || ext == "png" || ext == "bmp" || ext == "gif";
}

std::string ImageDataLoader::getExtension(const std::string& path) const {
    size_t pos = path.find_last_of('.');
    if (pos == std::string::npos) return "";
    std::string ext = path.substr(pos + 1);
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    return ext;
}

std::shared_ptr<IData> ImageDataLoader::load(const std::string& path) {
    std::string ext = getExtension(path);
    if (!supports(ext)) {
        std::cerr << "Erreur: format non supporté pour " << path << "\n";
        return nullptr;
    }

    int width, height, channels;
    unsigned char* img = stbi_load(path.c_str(), &width, &height, &channels, 0);
    if (!img) {
        std::cerr << "Erreur: impossible de lire l'image " << path << "\n";
        return nullptr;
    }

    ImageFormat format = ImageFormat::JPG;
    if (ext == "png") format = ImageFormat::PNG;
    else if (ext == "bmp") format = ImageFormat::BMP;
    else if (ext == "gif") format = ImageFormat::GIF;

    auto imageData = std::make_shared<ImageData>(width, height, format);

    // Remplir ImageData
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            Pixel& p = imageData->at(x, y);
            if (channels == 1) {
                uint8_t val = img[y * width + x];
                p.r = p.g = p.b = val;
                p.a = 255;
            } else if (channels == 3) {
                int idx = (y * width + x) * 3;
                p.r = img[idx];
                p.g = img[idx + 1];
                p.b = img[idx + 2];
                p.a = 255;
            } else if (channels == 4) {
                int idx = (y * width + x) * 4;
                p.r = img[idx];
                p.g = img[idx + 1];
                p.b = img[idx + 2];
                p.a = img[idx + 3];
            }
        }
    }

    stbi_image_free(img);
    return imageData;
}
