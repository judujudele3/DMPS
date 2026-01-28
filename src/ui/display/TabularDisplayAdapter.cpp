
#include "TabularDisplayAdapter.hpp"
#include "../../data/TabularData.hpp"
#include "../../core/DataType.hpp"
#include <QStackedWidget> //  Indispensable pour gérer les pages
#include <QTableWidget>
#include <QHeaderView>
#include <QVBoxLayout>
#include <iostream>

bool TabularDisplayAdapter::canDisplay(const IData& data) const
{
    // On vérifie le type enum et on s'assure que le cast est possible
    return data.type() == DataType::Tabular &&
           dynamic_cast<const TabularData*>(&data) != nullptr;
}

void TabularDisplayAdapter::display(const IData& data, QWidget* container)
{
    // 1. Cast sécurisé des données
    const TabularData* tabularDataPtr = dynamic_cast<const TabularData*>(&data);
    if (!tabularDataPtr) return;

    // 2. Récupérer le QStackedWidget
    QStackedWidget* stacked = qobject_cast<QStackedWidget*>(container);
    if (!stacked) {
        std::cerr << "[TabularDisplayAdapter] Erreur : Container n'est pas un QStackedWidget" << std::endl;
        return;
    }

    std::cout << "[TabularDisplayAdapter] Nettoyage et affichage du tableau..." << std::endl;


    // NETTOYAGE
    // On supprime les anciennes pages
    while (stacked->count() > 0) {
        QWidget* w = stacked->widget(0);
        stacked->removeWidget(w);
        delete w;
    }

    // CRÉER UNE NOUVELLE PAGE
    QWidget* page = new QWidget(stacked);
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(0, 0, 0, 0); // Plein écran dans la zone

    //  CRÉER ET AJOUTER LE TABLEAU
    // On passe 'page' comme parent, pas 'container'
    QTableWidget* table = createTableWidget(*tabularDataPtr, page);

    layout->addWidget(table);


    // FINALISATION
    stacked->addWidget(page);
    stacked->setCurrentWidget(page);
}

QTableWidget* TabularDisplayAdapter::createTableWidget(const TabularData& tabularData, QWidget* parent)
{
    QTableWidget* table = new QTableWidget(parent);

    // Configuration du tableau (Style et Comportement)
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);  // Lecture seule
    table->setSelectionBehavior(QAbstractItemView::SelectRows); // Sélection par ligne
    table->setAlternatingRowColors(true);                       // Une ligne sur deux grise
    table->setShowGrid(true);

    // Peupler le tableau avec les données
    populateTable(table, tabularData);

    // Ajustement visuel (à faire APRÈS le remplissage pour bien calculer les tailles)
    table->resizeColumnsToContents();
    table->horizontalHeader()->setStretchLastSection(true);

    return table;
}

void TabularDisplayAdapter::populateTable(QTableWidget* table, const TabularData& tabularData)
{
    // Vérifiez si vos méthodes s'appellent header()/rows() ou getHeader()/getRows() dans TabularData.hpp
    const auto& header = tabularData.header();
    const auto& rows = tabularData.rows();

    // Définir la taille du tableau
    table->setRowCount(static_cast<int>(rows.size()));
    table->setColumnCount(static_cast<int>(header.size()));

    // Définir les titres des colonnes
    QStringList headerLabels;
    for (const auto& col : header) {
        headerLabels << QString::fromStdString(col);
    }
    table->setHorizontalHeaderLabels(headerLabels);

    // Remplir les cellules
    for (size_t row = 0; row < rows.size(); ++row) {
        for (size_t col = 0; col < rows[row].size(); ++col) {
            // Sécurité pour éviter le crash si une ligne est plus courte
            if (col < rows[row].size()) {
                QTableWidgetItem* item = new QTableWidgetItem(
                    QString::fromStdString(rows[row][col])
                    );
                item->setTextAlignment(Qt::AlignCenter); // Optionnel : centrer le texte
                table->setItem(static_cast<int>(row), static_cast<int>(col), item);
            }
        }
    }
}






