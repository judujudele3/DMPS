#include "TabularFilterNumericColumnsModule.hpp"
#include <cctype>

std::string TabularFilterNumericColumnsModule::getName() const {
    return "Filter Numeric Columns Module";
}

ModuleResult TabularFilterNumericColumnsModule::apply(IData& data) {
    ModuleResult res;
    if (data.type() != DataType::Tabular) {
        res.addError(ErrorType::Error, "INVALID_DATA_TYPE", "TabularFilterNumericColumnsModule expects TabularData.");
        return res;
    }

    TabularData& table = dynamic_cast<TabularData&>(data);
    std::vector<std::string> numericCols;

    for (const auto& col : table.header()) {
        bool isNumeric = true;
        int idx = table.columnIndex(col);
        for (const auto& row : table.rows()) {
            const std::string& val = row[idx];
            if (!val.empty()) {
                try { std::stod(val); } catch(...) { isNumeric = false; break; }
            }
        }
        if (isNumeric) numericCols.push_back(col);
    }

    table.setHeader(numericCols); // ne garde que les colonnes numériques
    res.message = "Kept " + std::to_string(numericCols.size()) + " numeric columns.";
    return res;
}
