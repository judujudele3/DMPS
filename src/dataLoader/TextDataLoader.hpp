#ifndef TEXTDATALOADER_HPP
#define TEXTDATALOADER_HPP

#pragma once
#include "IDataLoader.hpp"

class TextDataLoader : public IDataLoader
{
public:
    bool supports(const std::string& extension) const override;
    std::shared_ptr<IData> load(const std::string& path) override;
};


#endif // TEXTDATALOADER_HPP
