#include "TabularMaxModule.hpp"
#include <limits>

TabularMaxModule::TabularMaxModule(const std::string& colName)
    : m_colName(colName) {}

std::string TabularMaxModule::getName() const {
    return "Tabular Max Module";
}

ModuleResult TabularMaxModule::apply(IData& data) {
    ModuleResult res;

    if (data.type() != DataType::Tabular) {
        res.addError(ErrorType::Error, "INVALID_DATA_TYPE",
                     "TabularMaxModule expects TabularData.");
        return res;
    }

    TabularData& table = dynamic_cast<TabularData&>(data);
    if (!table.hasColumn(m_colName)) {
        res.addError(ErrorType::Error, "COLUMN_NOT_FOUND",
                     "Column not found: " + m_colName);
        return res;
    }

    double maxVal = std::numeric_limits<double>::lowest();
    for (const auto& row : table.rows()) {
        const std::string& val = row[table.columnIndex(m_colName)];
        try {
            double v = std::stod(val);
            if (v > maxVal) maxVal = v;
        } catch (...) { continue; }
    }

    res.set(m_colName + "_max", maxVal);
    res.message = "Max computed for column: " + m_colName;
    return res;
}
