#ifndef IMAGEDISPLAYADAPTER_HPP
#define IMAGEDISPLAYADAPTER_HPP

#include "IDisplayAdapter.hpp"
#include "../../data/ImageData.hpp"
#include <memory>
#include <QPixmap>


class QLabel;

class ImageDisplayAdapter : public IDisplayAdapter
{
public:
    ImageDisplayAdapter() = default;
    ~ImageDisplayAdapter() override = default;

    bool canDisplay(const IData& data) const override;
    void display(const IData& data, QWidget* container) override;

private:
    QLabel* createImageLabel(const ImageData& imageData, QWidget* parent);
    QPixmap convertToPixmap(const ImageData& imageData);
};


#endif // IMAGEDISPLAYADAPTER_HPP
