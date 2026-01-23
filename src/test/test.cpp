#include <iostream>
#include <chrono>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include "../data/tabulardata.hpp"
#include "../module/TabularDescribe.hpp"
#include "../module/TabularSumModule.hpp"
#include "../module/TabularCountModule.hpp"
#include "../module/TabularMeanModule.hpp"
#include "../module/TabularStdModule.hpp"
#include "../module/TabularMinModule.hpp"
#include "../module/TabularMaxModule.hpp"
#include "../module/TabularMedianModule.hpp"
#include "../module/TabularIsNullModule.hpp"

// Fonction pour créer un dataset simple
TabularData createDataset(int nCols, int nRows) {
    TabularData table(TableFormat::CSV);

    // Création des noms de colonnes : A, B, C, ... ou AA, AB, etc.
    std::vector<std::string> header;
    for (int c = 0; c < nCols; c++) {
        std::string colName;
        int temp = c;
        do {
            colName = char('A' + temp % 26) + colName;
            temp = temp / 26 - 1;
        } while (temp >= 0);
        header.push_back(colName);
    }
    table.setHeader(header);

    // Remplir les lignes avec des valeurs numériques simples
    for (int r = 0; r < nRows; r++) {
        std::vector<std::string> row;
        for (int c = 0; c < nCols; c++) {
            std::ostringstream oss;
            oss << std::fixed << std::setprecision(2) << double(r + c + 1);
            row.push_back(oss.str());
        }
        table.addRow(row);
    }

    return table;
}

// Fonction pour mesurer le temps d'exécution
long long measureTime(int nCols, int nRows) {
    TabularData table = createDataset(nCols, nRows);

    auto start = std::chrono::high_resolution_clock::now();

    TabularDescribe describeModule;
    ModuleResult result = describeModule.apply(table);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    return duration.count(); // temps en millisecondes
}

int main() {
    // Liste de datasets à tester : {colonnes, lignes}
    std::vector<std::pair<int,int>> tests = {
        {20,4}, {20,50}, {20,500}, {20,1000}, {20,1000},
        {20,5000}, {20,10000}, {20,50000}, {20,100000}, {20,200000}
    };

    std::cout << "Cols\tRows\tTemps(ms)\n";
    std::cout << "-----------------------------\n";

    for (auto [cols, rows] : tests) {
        long long timeMs = measureTime(cols, rows);
        std::cout << cols << "\t" << rows << "\t" << timeMs << "\n";
    }

    return 0;
}
