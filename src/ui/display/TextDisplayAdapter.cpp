#include "TextDisplayAdapter.hpp"

#include "../../data/TextData.hpp"

#include <QStackedWidget>
#include <QTextEdit>
#include <QVBoxLayout>

bool TextDisplayAdapter::canDisplay(const IData& data) const
{
    return dynamic_cast<const TextData*>(&data) != nullptr;
}

void TextDisplayAdapter::display(const IData& data, QWidget* container)
{
    const TextData& textData = static_cast<const TextData&>(data);

    auto* stacked = qobject_cast<QStackedWidget*>(container);
    if (!stacked)
        return;

    // Créer une page dédiée au texte
    QWidget* page = new QWidget(stacked);
    QVBoxLayout* layout = new QVBoxLayout(page);

    QTextEdit* textEdit = new QTextEdit(page);
    textEdit->setReadOnly(true);
    textEdit->setText(QString::fromStdString(textData.getContent()));

    layout->addWidget(textEdit);
    page->setLayout(layout);

    stacked->addWidget(page);
    stacked->setCurrentWidget(page);
}
