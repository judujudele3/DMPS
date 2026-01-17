#include "TextData.hpp"

TextData::TextData(std::string content, TextFormat format)
    : m_content(std::move(content)), m_format(format)
{}

DataType TextData::type() const {
    return DataType::Text;
}

const std::string& TextData::getContent() const {
    return m_content;
}

void TextData::setContent(const std::string& text) {
    m_content = text;
}

TextFormat TextData::format() const {
    return m_format;
}
