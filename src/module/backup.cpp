#include "TabularDescribe.hpp"
#include "TabularSumModule.hpp"
#include "TabularCountModule.hpp"
#include "TabularMeanModule.hpp"
#include "TabularStdModule.hpp"
#include "TabularMinModule.hpp"
#include "TabularMaxModule.hpp"
#include "TabularMedianModule.hpp"
#include "TabularIsNullModule.hpp"

std::string TabularDescribe::getName() const {
    return "Tabular Summary Module";
}

ModuleResult TabularDescribe::apply(IData& data) {
    ModuleResult res;
    if (data.type() != DataType::Tabular) {
        res.addError(ErrorType::Error, "INVALID_DATA_TYPE", "TabularSummaryModule expects TabularData.");
        return res;
    }

    TabularData& table = dynamic_cast<TabularData&>(data);

    // Boucle sur toutes les colonnes numériques
    for (const auto& col : table.header()) {
        int idx = table.columnIndex(col);
        bool isNumeric = true;
        for (const auto& row : table.rows()) {
            if (!row[idx].empty()) {
                try { std::stod(row[idx]); } catch(...) { isNumeric = false; break; }
            }
        }
        if (!isNumeric) continue;

        // Calculer les stats
        TabularSumModule sumMod(col);
        TabularCountModule countMod(col);
        TabularMeanModule meanMod(col);
        TabularStdModule stdMod(col);
        TabularMinModule minMod(col);
        TabularMaxModule maxMod(col);
        TabularMedianModule medianMod(col);
        TabularIsNullModule isNullMod(col);

        res.set(col + "_sum", sumMod.apply(table).get<double>("sum"));
        res.set(col + "_count", countMod.apply(table).get<int>("count"));
        res.set(col + "_mean", meanMod.apply(table).get<double>("mean"));
        res.set(col + "_std", stdMod.apply(table).get<double>("std"));
        res.set(col + "_min", minMod.apply(table).get<double>("min"));
        res.set(col + "_max", maxMod.apply(table).get<double>("max"));
        res.set(col + "_median", medianMod.apply(table).get<double>("median"));
        res.set(col + "_isnull", isNullMod.apply(table).get<int>("isnull"));

    }

    res.message = "Summary computed for numeric columns.";
    return res;
}


//// methode avec le threads+mutex