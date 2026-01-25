#ifndef IDISPLAYADAPTER_HPP
#define IDISPLAYADAPTER_HPP

#pragma once

class IData;
class QWidget;

class IDisplayAdapter
{
public:
    virtual ~IDisplayAdapter() = default;

    virtual bool canDisplay(const IData& data) const = 0;
    virtual void display(const IData& data, QWidget* container) = 0;
};


#endif // IDISPLAYADAPTER_HPP
