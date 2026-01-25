#ifndef TEXTDISPLAYADAPTER_HPP
#define TEXTDISPLAYADAPTER_HPP

#pragma once

#include "IDisplayAdapter.hpp"

class TextDisplayAdapter : public IDisplayAdapter
{
public:
    bool canDisplay(const IData& data) const override;
    void display(const IData& data, QWidget* container) override;
};

#endif // TEXTDISPLAYADAPTER_HPP
