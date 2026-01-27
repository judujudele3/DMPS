#include "TabularDisplayAdapter.hpp"
#include "../../data/IData.hpp"
#include "../../core/DataType.hpp"
#include <QTableWidget>
#include <QHeaderView>
#include <QVBoxLayout>

bool TabularDisplayAdapter::canDisplay(const IData& data) const
{
    return data.type() == DataType::Tabular;
}

void TabularDisplayAdapter::display(const IData& data, QWidget* container)
{
    if (!canDisplay(data)) {
        return;
    }

    const TabularData& tabularData = dynamic_cast<const TabularData&>(data);

    // Nettoyer le container
    QLayout* oldLayout = container->layout();
    if (oldLayout) {
        QWidget().setLayout(oldLayout);
    }

    // Créer un layout vertical
    QVBoxLayout* layout = new QVBoxLayout(container);
    layout->setContentsMargins(0, 0, 0, 0);

    // Créer le tableau
    QTableWidget* table = createTableWidget(tabularData, container);
    layout->addWidget(table);
}

QTableWidget* TabularDisplayAdapter::createTableWidget(const TabularData& tabularData, QWidget* parent)
{
    QTableWidget* table = new QTableWidget(parent);

    // Configuration du tableau
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);  // Read-only
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setAlternatingRowColors(true);

    // Ajuster les colonnes automatiquement
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    table->horizontalHeader()->setStretchLastSection(true);

    // Peupler le tableau
    populateTable(table, tabularData);

    return table;
}

void TabularDisplayAdapter::populateTable(QTableWidget* table, const TabularData& tabularData)
{
    const auto& header = tabularData.header();
    const auto& rows = tabularData.rows();

    // Définir le nombre de lignes et colonnes
    table->setRowCount(static_cast<int>(rows.size()));
    table->setColumnCount(static_cast<int>(header.size()));

    // Définir les headers
    QStringList headerLabels;
    for (const auto& col : header) {
        headerLabels << QString::fromStdString(col);
    }
    table->setHorizontalHeaderLabels(headerLabels);

    // Remplir les données
    for (size_t row = 0; row < rows.size(); ++row) {
        for (size_t col = 0; col < rows[row].size(); ++col) {
            QTableWidgetItem* item = new QTableWidgetItem(
                QString::fromStdString(rows[row][col])
                );
            table->setItem(static_cast<int>(row), static_cast<int>(col), item);
        }
    }
}
