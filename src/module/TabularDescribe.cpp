#include "TabularDescribe.hpp"
#include "TabularSumModule.hpp"
#include "TabularCountModule.hpp"
#include "TabularMeanModule.hpp"
#include "TabularStdModule.hpp"
#include "TabularMinModule.hpp"
#include "TabularMaxModule.hpp"
#include "TabularMedianModule.hpp"
#include "TabularIsNullModule.hpp"
#include <thread>
#include <mutex>
#include <algorithm>
#include <numeric>
#include <cmath>
std::string TabularDescribe::getName() const {
    return "Tabular Summary Module";
}

ModuleResult TabularDescribe::apply(IData& data) {
    ModuleResult res;

    if (data.type() != DataType::Tabular) {
        res.addError(ErrorType::Error, "INVALID_DATA_TYPE", "TabularDescribe expects TabularData.");
        return res;
    }

    TabularData& table = dynamic_cast<TabularData&>(data);
    std::mutex mtx; // pour protéger l'écriture dans res

    // Fonction pour traiter une colonne
    auto processColumn = [&](const std::string& col) {
        int idx = table.columnIndex(col);

        std::vector<double> values;
        int isNull = 0;

        // 1 passe pour récupérer les valeurs numériques et compter les nulls
        for (const auto& row : table.rows()) {
            if (idx >= row.size() || row[idx].empty()) {
                isNull++;
            } else {
                try {
                    values.push_back(std::stod(row[idx]));
                } catch (...) {
                    isNull++;
                }
            }
        }

        if (values.empty()) return; // Pas de valeurs numériques, on ignore la colonne

        // Calcul des stats
        double sum = std::accumulate(values.begin(), values.end(), 0.0);
        int count = values.size();
        double mean = sum / count;

        double sqSum = 0.0;
        for (double v : values) sqSum += (v - mean) * (v - mean);
        double stdDev = std::sqrt(sqSum / count);

        double minVal = *std::min_element(values.begin(), values.end());
        double maxVal = *std::max_element(values.begin(), values.end());

        // Médiane
        std::sort(values.begin(), values.end());
        double med = (count % 2 == 1) ? values[count/2] : (values[count/2-1] + values[count/2]) / 2.0;

        // Écriture thread-safe dans le résultat
        std::lock_guard<std::mutex> lock(mtx);
        res.set(col + "_sum", sum);
        res.set(col + "_count", count);
        res.set(col + "_mean", mean);
        res.set(col + "_std", stdDev);
        res.set(col + "_min", minVal);
        res.set(col + "_max", maxVal);
        res.set(col + "_median", med);
        res.set(col + "_isnull", isNull);
    };

    // Créer un thread par colonne
    std::vector<std::thread> threads;
    for (const auto& col : table.header()) {
        threads.emplace_back(processColumn, col);
    }

    // Attendre la fin de tous les threads
    for (auto& t : threads) t.join();

    res.message = "Summary computed for numeric columns (Optimized + Multithreaded).";
    return res;
}
