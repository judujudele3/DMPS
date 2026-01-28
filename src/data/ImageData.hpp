#ifndef IMAGEDATA_H
#define IMAGEDATA_H

#include "IData.hpp"
#include <vector>
#include <cstdint>

enum class ImageFormat {
    JPG,
    PNG,
    BMP,
    GIF
};

struct Pixel {
    uint8_t r, g, b, a;
};

class ImageData : public IData {
public:
    ImageData(int width, int height, ImageFormat format);

    DataType type() const override;

    int getWidth() const;
    int getHeight() const;

    int& getWidth() ;
    int& getHeight() ;
    ImageFormat format() const;

    Pixel& at(int x, int y);
    const Pixel& at(int x, int y) const;

    //pour accèder au pixels
    std::vector<Pixel>& pixels();
    const std::vector<Pixel>& pixels() const;


private:
    int m_width;
    int m_height;
    ImageFormat m_format;
    std::vector<Pixel> m_pixels;
};

#endif
