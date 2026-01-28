#include "ImageDataLoader.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"
#include <iostream>
#include <algorithm>

bool ImageDataLoader::supports(const std::string& extension) const {
    std::string ext = extension;
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

    return ext == ".jpg" || ext == ".jpeg" || ext == ".png"
           ||ext == ".bmp" || ext == ".gif";
}

std::string ImageDataLoader::getExtension(const std::string& path) const {
    size_t pos = path.find_last_of('.');
    if (pos == std::string::npos) return "";

    std::string ext = path.substr(pos);
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
    unsigned char* img = stbi_load(path.c_str(), &width, &height, &channels, 4);

    if (!img) {
        std::cerr << "Erreur STB: " << stbi_failure_reason() << "\n";
        std::cerr << "Impossible de lire l'image " << path << "\n";
        return nullptr;
    }

    std::cout << "Image Loaded: " << width << "x" << height << " (" << channels << " canaux)\n";

    // Determine the format
    ImageFormat format = ImageFormat::PNG;
    if (ext == ".jpg" || ext == ".jpeg") format = ImageFormat::JPG;
    else if (ext == ".bmp") format = ImageFormat::BMP;
    else if (ext == ".gif") format = ImageFormat::GIF;

    auto imageData = std::make_shared<ImageData>(width, height, format);

    // Get a reference to the pixels
    std::vector<Pixel>& pixels = imageData->pixels();

    // Check the size
    size_t expectedSize = static_cast<size_t>(width) * static_cast<size_t>(height);
    if (pixels.size() != expectedSize) {
        std::cerr << "ERREUR: Taille pixels incorrecte!" << std::endl;
        stbi_image_free(img);
        return nullptr;
    }

    // Secure filling
    size_t pixelIndex = 0;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            size_t imgIndex = (static_cast<size_t>(y) * static_cast<size_t>(width) + static_cast<size_t>(x)) * 4;

            pixels[pixelIndex].r = img[imgIndex];
            pixels[pixelIndex].g = img[imgIndex + 1];
            pixels[pixelIndex].b = img[imgIndex + 2];
            pixels[pixelIndex].a = img[imgIndex + 3];

            pixelIndex++;
        }
    }

    stbi_image_free(img);

    std::cout << "ImageData Created " << std::endl;

    return imageData;
}




/*

std::shared_ptr<IData> ImageDataLoader::load(const std::string& path) {
    std::string ext = getExtension(path);

    if (!supports(ext)) {
        std::cerr << "Erreur: format non supporté pour " << path << "\n";
        return nullptr;
    }

    int width, height, channels;
    unsigned char* img = stbi_load(path.c_str(), &width, &height, &channels, 4); // RGBA

    if (!img) {
        std::cerr << "Erreur STB: " << stbi_failure_reason() << "\n";
        std::cerr << "Impossible de lire l'image " << path << "\n";
        return nullptr;
    }

    std::cout << "Image chargée: " << width << "x" << height << " (" << channels << " canaux)\n";

    // Déterminer le format
    ImageFormat format = ImageFormat::PNG;
    if (ext == "jpg" || ext == "jpeg") format = ImageFormat::JPG;
    else if (ext == "bmp") format = ImageFormat::BMP;
    else if (ext == "gif") format = ImageFormat::GIF;

    auto imageData = std::make_shared<ImageData>(width, height, format);
    auto& pixels = imageData->pixels();

    // ⚠️ Redimensionner explicitement le vecteur pour éviter out-of-bounds
    pixels.resize(width * height);




    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int idx = (y * width + x) * 4; // RGBA
            pixels[y * width + x] = Pixel{
                img[idx],     // R
                img[idx + 1], // G
                img[idx + 2], // B
                img[idx + 3]  // A
            };
        }
    }

    stbi_image_free(img);

    std::cout << "ImageData créée avec " << pixels.size() << " pixels\n";

    return imageData;
}
*/
