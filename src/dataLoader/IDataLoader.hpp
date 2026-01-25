#ifndef IDATALOADER_HPP
#define IDATALOADER_HPP

#pragma once
#include <string>
#include <memory>
#include "../data/IData.hpp"

class IDataLoader
{
public:
    virtual ~IDataLoader() = default;
    virtual bool supports(const std::string& extension) const = 0;
    virtual std::shared_ptr<IData> load(const std::string& path) = 0;
};


#endif // IDATALOADER_HPP
