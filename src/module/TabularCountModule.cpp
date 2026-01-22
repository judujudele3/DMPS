#include "TabularCountModule.hpp"

TabularCountModule::TabularCountModule(const std::string& colName)
    : m_colName(colName) {}

std::string TabularCountModule::getName() const {
    return "Tabular Count Module";
}

ModuleResult TabularCountModule::apply(IData& data) {
    ModuleResult res;

    if (data.type() != DataType::Tabular) {
        res.addError(ErrorType::Error, "INVALID_DATA_TYPE",
                     "TabularCountModule expects TabularData.");
        return res;
    }

    TabularData& table = dynamic_cast<TabularData&>(data);

    if (!table.hasColumn(m_colName)) {
        res.addError(ErrorType::Error, "COLUMN_NOT_FOUND",
                     "Column not found: " + m_colName);
        return res;
    }

    int count = 0;
    for (const auto& row : table.rows()) {
        const std::string& val = row[table.columnIndex(m_colName)];
        if (!val.empty()) count++;
    }

    res.set(m_colName + "_count", count);
    res.message = "Count computed for column: " + m_colName;
    return res;
}
