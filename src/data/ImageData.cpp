#include "imagedata.hpp"


ImageData::ImageData(int width, int height ,ImageFormat format):
    m_width(width),
    m_height(height),
    m_format(format),
    m_pixels(width*height,{0,0,0,255})

{}


DataType ImageData::type() const {
    return DataType::Image;
}

int ImageData::getWidth() const{
    return m_width;
}

int ImageData::getHeight() const {
    return m_height;
}


int& ImageData::getWidth() {
    return m_width;
}

int& ImageData::getHeight()  {
    return m_height;
}


ImageFormat ImageData::format() const {
    return m_format;
}

Pixel& ImageData::at(int x, int y) {
    return m_pixels[y * m_width + x];
}

const Pixel& ImageData::at(int x, int y) const {
    return m_pixels[y * m_width + x];
}

//pour accèder au pixels
std::vector<Pixel>& ImageData::pixels() { return m_pixels; }
const std::vector<Pixel>& ImageData::pixels() const { return m_pixels; }

