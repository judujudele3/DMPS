#include "ImageDataSaver.hpp"

bool ImageDataSaver::save(const IData& data, const std::string& path) {
    const ImageData* imgData = dynamic_cast<const ImageData*>(&data);
    if (!imgData) {
        std::cerr << "Error: Provided data is not ImageData.\n";
        return false;
    }

    int width = imgData->getWidth();
    int height = imgData->getHeight();
    std::vector<unsigned char> buffer(width * height * 3);

    // Fill buffer RGB
    const auto& pixels = imgData->pixels();
    for (int i = 0; i < width * height; ++i) {
        buffer[i * 3 + 0] = pixels[i].r;
        buffer[i * 3 + 1] = pixels[i].g;
        buffer[i * 3 + 2] = pixels[i].b;
    }

    std::string ext = path.substr(path.find_last_of('.') + 1);
    int result = 0;
    if (ext == "png") {
        result = stbi_write_png(path.c_str(), width, height, 3, buffer.data(), width * 3);
    } else if (ext == "jpg") {
        result = stbi_write_jpg(path.c_str(), width, height, 3, buffer.data(), 100);
    } else if (ext == "bmp") {
        result = stbi_write_bmp(path.c_str(), width, height, 3, buffer.data());
    } else {
        std::cerr << "Error: Unsupported image format ." << ext << "\n";
        return false;
    }

    if (!result) {
        std::cerr << "Error: Failed to write image to " << path << "\n";
        return false;
    }

    std::cout << "Image saved successfully to " << path << "\n";
    return true;
}
