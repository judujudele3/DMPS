#include "TabularSumModule.hpp"
#include "../core/ModuleError.hpp"
#include <cstdlib>   // std::stod

TabularSumModule::TabularSumModule(const std::string& columnName)
    : m_columnName(columnName) {}

std::string TabularSumModule::getName() const {
    return "TabularSumModule";
}

ModuleResult TabularSumModule::apply(IData& data) {
    ModuleResult res;

    // Vérifier le type
    if (data.type() != DataType::Tabular) {
        res.addError(ErrorType::Error,
                     "INVALID_DATA_TYPE",
                     "TabularSumModule expects TabularData");
        return res;
    }

    // Cast sécurisé
    TabularData& table = dynamic_cast<TabularData&>(data);

    // Vérifier colonne
    if (!table.hasColumn(m_columnName)) {
        res.addError(ErrorType::Error,
                     "COLUMN_NOT_FOUND",
                     "Column not found: " + m_columnName);
        return res;
    }

    int colIdx = table.columnIndex(m_columnName);
    double sum = 0.0;
    size_t count = 0;

    // Calcul
    for (const auto& row : table.rows()) {
        if (colIdx >= static_cast<int>(row.size()))
            continue;

        try {
            double val = std::stod(row[colIdx]);
            sum += val;
            ++count;
        } catch (...) {
            // ignorer non numériques
        }
    }

    // Résultat
    res.set("column", m_columnName);
    res.set( "sum", sum);
    res.set("count", static_cast<int>(count));

    res.message = "Sum computed successfully";

    return res;
}

std::vector<ModuleParameter> TabularSumModule::getParameterDescriptors() {
    return {
        {"columnName", "string", std::string(""), "Nom de la colonne à sommer"}
    };
}
