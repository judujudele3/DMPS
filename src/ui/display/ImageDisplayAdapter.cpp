#include "ImageDisplayAdapter.hpp"
#include "../../data/IData.hpp"
#include "../../core/DataType.hpp"
#include <QLabel>
#include <QPixmap>
#include <QImage>
#include <QVBoxLayout>
#include <QScrollArea>
#include <iostream>

bool ImageDisplayAdapter::canDisplay(const IData& data) const
{
    std::cout << "[ImageDisplayAdapter] canDisplay() appelé" << std::endl;
    std::cout << "[ImageDisplayAdapter] Type de donnée: " << static_cast<int>(data.type()) << std::endl;
    std::cout << "[ImageDisplayAdapter] DataType::Image vaut: " << static_cast<int>(DataType::Image) << std::endl;

    bool result = (data.type() == DataType::Image);
    std::cout << "[ImageDisplayAdapter] canDisplay result: " << result << std::endl;

    return result;
}

void ImageDisplayAdapter::display(const IData& data, QWidget* container)
{
    std::cout << "=== ImageDisplayAdapter::display ===" << std::endl;

    if (!canDisplay(data)) {
        std::cout << "[ImageDisplayAdapter] ERROR: canDisplay() returned false!" << std::endl;
        return;
    }

    std::cout << "[ImageDisplayAdapter] Data type OK, casting to ImageData..." << std::endl;
    const ImageData& imageData = dynamic_cast<const ImageData&>(data);

    std::cout << "[ImageDisplayAdapter] Cast réussi!" << std::endl;
    std::cout << "[ImageDisplayAdapter] Creating layout..." << std::endl;

    // Nettoyer le container
    QLayout* oldLayout = container->layout();
    if (oldLayout) {
        std::cout << "[ImageDisplayAdapter] Deleting old layout" << std::endl;
        QWidget().setLayout(oldLayout);
    }

    QVBoxLayout* layout = new QVBoxLayout(container);
    layout->setContentsMargins(0, 0, 0, 0);
    std::cout << "[ImageDisplayAdapter] Layout créé" << std::endl;

    QScrollArea* scrollArea = new QScrollArea(container);
    scrollArea->setWidgetResizable(true);
    std::cout << "[ImageDisplayAdapter] ScrollArea créée" << std::endl;

    std::cout << "[ImageDisplayAdapter] Converting to QPixmap..." << std::endl;
    QLabel* imageLabel = createImageLabel(imageData, scrollArea);

    std::cout << "[ImageDisplayAdapter] Label created, setting widget..." << std::endl;
    scrollArea->setWidget(imageLabel);
    layout->addWidget(scrollArea);

    std::cout << "[ImageDisplayAdapter] Display complete!" << std::endl;
}

QLabel* ImageDisplayAdapter::createImageLabel(const ImageData& imageData, QWidget* parent)
{
    std::cout << "[ImageDisplayAdapter] createImageLabel() appelé" << std::endl;

    QLabel* label = new QLabel(parent);
    label->setAlignment(Qt::AlignCenter);
    std::cout << "[ImageDisplayAdapter] QLabel créé" << std::endl;

    QPixmap pixmap = convertToPixmap(imageData);
    std::cout << "[ImageDisplayAdapter] QPixmap obtenu, taille: "
              << pixmap.width() << "x" << pixmap.height() << std::endl;

    if (pixmap.isNull()) {
        std::cout << "[ImageDisplayAdapter] ERROR: QPixmap is NULL!" << std::endl;
        label->setText("Error: Could not load image");
    } else {
        label->setPixmap(pixmap);
        std::cout << "[ImageDisplayAdapter] Pixmap set to label" << std::endl;
    }

    return label;
}

QPixmap ImageDisplayAdapter::convertToPixmap(const ImageData& imageData)
{
    std::cout << "[ImageDisplayAdapter] convertToPixmap() appelé" << std::endl;

    int width = const_cast<ImageData&>(imageData).getWidth();
    int height = const_cast<ImageData&>(imageData).getHeight();
    const auto& pixels = imageData.pixels();

    std::cout << "[ImageDisplayAdapter] Dimensions image: " << width << "x" << height << std::endl;
    std::cout << "[ImageDisplayAdapter] Nombre de pixels: " << pixels.size() << std::endl;
    std::cout << "[ImageDisplayAdapter] Pixels attendus: " << (width * height) << std::endl;

    if (pixels.empty()) {
        std::cout << "[ImageDisplayAdapter] ERROR: No pixels to display!" << std::endl;
        return QPixmap();
    }

    if (pixels.size() != static_cast<size_t>(width * height)) {
        std::cout << "[ImageDisplayAdapter] WARNING: Pixel count mismatch!" << std::endl;
    }

    std::cout << "[ImageDisplayAdapter] Création de la QImage..." << std::endl;
    QImage image(width, height, QImage::Format_RGBA8888);

    if (image.isNull()) {
        std::cout << "[ImageDisplayAdapter] ERROR: Could not create QImage!" << std::endl;
        return QPixmap();
    }

    std::cout << "[ImageDisplayAdapter] Copie des pixels dans QImage..." << std::endl;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            const Pixel& pixel = pixels[y * width + x];
            image.setPixel(x, y, qRgba(pixel.r, pixel.g, pixel.b, pixel.a));
        }
    }

    std::cout << "[ImageDisplayAdapter] Pixels copiés, première couleur: "
              << "R=" << (int)pixels[0].r
              << " G=" << (int)pixels[0].g
              << " B=" << (int)pixels[0].b
              << " A=" << (int)pixels[0].a << std::endl;

    std::cout << "[ImageDisplayAdapter] QImage créée, conversion vers QPixmap..." << std::endl;
    QPixmap pixmap = QPixmap::fromImage(image);

    std::cout << "[ImageDisplayAdapter] QPixmap size: "
              << pixmap.width() << "x" << pixmap.height() << std::endl;

    if (pixmap.isNull()) {
        std::cout << "[ImageDisplayAdapter] ERROR: QPixmap conversion failed!" << std::endl;
    } else {
        std::cout << "[ImageDisplayAdapter] QPixmap créé avec succès!" << std::endl;
    }

    return pixmap;
}
