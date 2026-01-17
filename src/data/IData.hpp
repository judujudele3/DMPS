#ifndef IDATA_HPP
#define IDATA_HPP
#pragma once
#include "../core/DataType.hpp"




class IData {
public:
    virtual ~IData() = default;
    virtual DataType type() const = 0;
};


#endif // IDATA_HPP
