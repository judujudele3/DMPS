#include "ImageDisplayAdapter.hpp"
#include "../../data/ImageData.hpp"
#include "../../data/IData.hpp"
#include "../../core/DataType.hpp"
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QVBoxLayout>
#include <QWheelEvent>
#include <QStackedWidget>
#include <QDebug>
#include <iostream>

// Classe custom pour gérer le zoom à la molette
class ZoomableGraphicsView : public QGraphicsView
{
public:
    ZoomableGraphicsView(QWidget* parent = nullptr) : QGraphicsView(parent)
    {
        setDragMode(QGraphicsView::ScrollHandDrag);
        setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    }

protected:
    void wheelEvent(QWheelEvent* event) override
    {
        // Zoom avec la molette
        double scaleFactor = 1.15;

        if (event->angleDelta().y() > 0) {
            // Zoom in
            scale(scaleFactor, scaleFactor);
        } else {
            // Zoom out
            scale(1.0 / scaleFactor, 1.0 / scaleFactor);
        }
    }
};

bool ImageDisplayAdapter::canDisplay(const IData& data) const
{
    if (data.type() != DataType::Image)
        return false;
    return dynamic_cast<const ImageData*>(&data) != nullptr;
}

void ImageDisplayAdapter::display(const IData& data, QWidget* container)
{
    std::cout << "[ImageDisplayAdapter] display() appelé" << std::endl;

    if (!canDisplay(data)) {
        std::cout << "[ImageDisplayAdapter] Type incompatible" << std::endl;
        return;
    }

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

    // Créer la vue zoomable (au lieu de QScrollArea + QLabel)
    QGraphicsView* view = createImageView(imageData, page);
    layout->addWidget(view);

    // Ajouter la page au stackedWidget
    stacked->addWidget(page);
    stacked->setCurrentWidget(page);

    std::cout << "[DEBUG] StackedWidget pages after: " << stacked->count() << std::endl;
    std::cout << "[ImageDisplayAdapter] Affichage terminé (zoomable)" << std::endl;
}

QGraphicsView* ImageDisplayAdapter::createImageView(const ImageData& imageData, QWidget* parent)
{
    // Créer la scène
    QGraphicsScene* scene = new QGraphicsScene(parent);

    // Convertir ImageData en QPixmap
    QPixmap pixmap = convertToPixmap(imageData);

    if (pixmap.isNull()) {
        std::cout << "[ImageDisplayAdapter] ERROR: QPixmap null" << std::endl;
        return new ZoomableGraphicsView(parent);
    }

    // Ajouter l'image à la scène
    scene->addPixmap(pixmap);

    // Créer la vue zoomable
    ZoomableGraphicsView* view = new ZoomableGraphicsView(parent);
    view->setScene(scene);

    // Ajuster la vue pour que l'image soit visible entièrement au départ
    view->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);

    std::cout << "[ImageDisplayAdapter] GraphicsView créée avec zoom" << std::endl;

    return view;
}

QPixmap ImageDisplayAdapter::convertToPixmap(const ImageData& imageData)
{
    // Utiliser la méthode améliorée du V2 avec setPixelColor
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
