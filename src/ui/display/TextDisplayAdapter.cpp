#include "TextDisplayAdapter.hpp"
#include "../../data/TextData.hpp"
#include "../../core/DataType.hpp"
#include <QStackedWidget>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QFont>
#include <QWheelEvent>
#include <iostream>

// Classe custom pour le zoom texte
class ZoomableTextEdit : public QTextEdit
{
public:
    ZoomableTextEdit(QWidget* parent = nullptr) : QTextEdit(parent) {}

protected:
    void wheelEvent(QWheelEvent* event) override
    {
        if (event->modifiers() & Qt::ControlModifier) {
            // Ctrl + Molette = Zoom
            if (event->angleDelta().y() > 0) {
                zoomIn(2);
            } else {
                zoomOut(2);
            }
            event->accept();
        } else {
            // Molette normale = Scroll
            QTextEdit::wheelEvent(event);
        }
    }
};

bool TextDisplayAdapter::canDisplay(const IData& data) const
{
    // Vérification stricte : Type enum ET cast dynamique
    return data.type() == DataType::Text &&
           dynamic_cast<const TextData*>(&data) != nullptr;
}

void TextDisplayAdapter::display(const IData& data, QWidget* container)
{
    // 1. Sécuriser le cast des données
    const TextData* textDataPtr = dynamic_cast<const TextData*>(&data);
    if (!textDataPtr) return;

    // 2. Vérifier et récupérer le QStackedWidget
    QStackedWidget* stacked = qobject_cast<QStackedWidget*>(container);
    if (!stacked) {
        std::cerr << "[TextDisplayAdapter] Erreur : Le conteneur n'est pas un QStackedWidget." << std::endl;
        return;
    }

    std::cout << "[TextDisplayAdapter] Nettoyage et affichage du texte..." << std::endl;

    // ---------------------------------------------------------
    // NETTOYAGE
    // On retire tous les widgets existants (Image, Tableau, ou vieux Texte)
    while (stacked->count() > 0) {
        QWidget* w = stacked->widget(0);
        stacked->removeWidget(w);
        delete w;
    }

    // CRÉER UNE NOUVELLE PAGE
    QWidget* page = new QWidget(stacked);
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(0, 0, 0, 0); // Pas de marges pour occuper tout l'espace

    // Création du composant d'édition de texte ZOOMABLE (Lecture seule)
    ZoomableTextEdit* textEdit = new ZoomableTextEdit(page);
    textEdit->setReadOnly(true);
    textEdit->setText(QString::fromStdString(textDataPtr->getContent()));

    // Style : Police monospace pour bien voir l'alignement (comme dans un éditeur de code)
    QFont font("Consolas", 10);
    if (font.exactMatch()) {
        textEdit->setFont(font);
    } else {
        textEdit->setFont(QFont("Courier New", 10)); // Fallback
    }

    // Ajouter le textEdit au layout de la page
    layout->addWidget(textEdit);

    // AJOUTER AU STACK ET AFFICHER
    stacked->addWidget(page);
    stacked->setCurrentWidget(page);

    std::cout << "[TextDisplayAdapter] Texte affiché avec zoom (Ctrl+Molette)" << std::endl;
}
