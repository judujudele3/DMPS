
#include "ImageDisplayAdapter.hpp"
#include "../../data/ImageData.hpp"
#include "../../data/IData.hpp"
#include "../../core/DataType.hpp"
#include <QLabel>
#include <QPixmap>
#include <QImage>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QStackedWidget>
#include <QDebug>
#include <iostream>


bool ImageDisplayAdapter::canDisplay(const IData& data) const
{
    if (data.type() != DataType::Image)
        return false;

    return dynamic_cast<const ImageData*>(&data) != nullptr;
}


void ImageDisplayAdapter::display(const IData& data, QWidget* container)
{
    // Cast sécurisé
    const ImageData& imageData = dynamic_cast<const ImageData&>(data);

    // Vérifier que le container est bien un QStackedWidget
    QStackedWidget* stacked = qobject_cast<QStackedWidget*>(container);
    if (!stacked) {
        qDebug() << "[DEBUG] Container is NOT a QStackedWidget";
        return;
    }

    std::cout << "[DEBUG] ImageDisplayAdapter::display called" << std::endl;
    std::cout << "[DEBUG] StackedWidget pages before: " << stacked->count() << std::endl;

    // Supprimer les anciennes pages
    while (stacked->count() > 0) {
        QWidget* w = stacked->widget(0);
        stacked->removeWidget(w);
        delete w;
    }

    // Créer une nouvelle page
    QWidget* page = new QWidget(stacked);
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(0, 0, 0, 0);

    // Créer QScrollArea pour gérer les grandes images
    QScrollArea* scrollArea = new QScrollArea(page);
    scrollArea->setAlignment(Qt::AlignCenter);
    scrollArea->setWidgetResizable(false); // Important: ne pas auto-redimensionner

    // Créer QLabel et l'ajouter au scrollArea
    QLabel* imageLabel = createImageLabel(imageData, scrollArea);
    scrollArea->setWidget(imageLabel);

    // Ajouter le scrollArea au layout (pas directement le label)
    layout->addWidget(scrollArea);

    // Ajouter la page au stackedWidget
    stacked->addWidget(page);
    stacked->setCurrentWidget(page);

    std::cout << "[DEBUG] StackedWidget pages after: " << stacked->count() << std::endl;
}


QPixmap ImageDisplayAdapter::convertToPixmap(const ImageData& imageData)
{
    QImage img(
        imageData.getWidth(),
        imageData.getHeight(),
        QImage::Format_RGBA8888
        );

    for (int y = 0; y < imageData.getHeight(); ++y) {
        for (int x = 0; x < imageData.getWidth(); ++x) {
            const Pixel& p = imageData.at(x, y);
            img.setPixelColor(x, y, QColor(p.r, p.g, p.b, p.a));
        }
    }

    QPixmap pixmap = QPixmap::fromImage(img);
    std::cout << "[DEBUG] Pixmap size: "
              << pixmap.width() << "x" << pixmap.height()
              << " | isNull: " << pixmap.isNull() << std::endl;
    return pixmap;
}


QLabel* ImageDisplayAdapter::createImageLabel(
    const ImageData& imageData,
    QWidget* parent
    ) {
    QLabel* label = new QLabel(parent);
    QPixmap pix = convertToPixmap(imageData);

    if (pix.isNull()) {
        label->setText("Erreur: impossible d'afficher l'image");
        qDebug() << "Pixmap NULL!";
    } else {
        label->setPixmap(pix);
    }

    label->setScaledContents(true); // Pour s'adapter à la taille du container
    label->setAlignment(Qt::AlignCenter);

    return label;
}
