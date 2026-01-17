#ifndef TEXTDATA_HPP
#define TEXTDATA_HPP

#include "IData.hpp"
#include <string>

enum class TextFormat { TXT, MD };

class TextData : public IData {
public:
    explicit TextData(std::string content, TextFormat format = TextFormat::TXT);

    DataType type() const override;

    const std::string& getContent() const;
    void setContent(const std::string& text);

    TextFormat format() const;

private:
    std::string m_content;
    TextFormat m_format;
};

#endif
