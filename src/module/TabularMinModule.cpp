#include "TabularMinModule.hpp"
#include <limits>

TabularMinModule::TabularMinModule(const std::string& colName)
    : m_colName(colName) {}

std::string TabularMinModule::getName() const {
    return "Tabular Min Module";
}

ModuleResult TabularMinModule::apply(IData& data) {
    ModuleResult res;

    if (data.type() != DataType::Tabular) {
        res.addError(ErrorType::Error, "INVALID_DATA_TYPE",
                     "TabularMinModule expects TabularData.");
        return res;
    }

    TabularData& table = dynamic_cast<TabularData&>(data);
    if (!table.hasColumn(m_colName)) {
        res.addError(ErrorType::Error, "COLUMN_NOT_FOUND",
                     "Column not found: " + m_colName);
        return res;
    }

    double minVal = std::numeric_limits<double>::max();
    for (const auto& row : table.rows()) {
        const std::string& val = row[table.columnIndex(m_colName)];
        try {
            double v = std::stod(val);
            if (v < minVal) minVal = v;
        } catch (...) { continue; }
    }

    res.set("min", minVal);
    res.message = "Min computed for column: " + m_colName;
    return res;
}


std::vector<ModuleParameter> TabularMinModule::getParameterDescriptors() {
    return {
        {"colName", "string", std::string(""), "Nom de la colonne pour laquelle calculer le minimum"}
    };
}
