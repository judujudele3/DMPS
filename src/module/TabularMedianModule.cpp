#include "TabularMedianModule.hpp"
#include <vector>
#include <algorithm>

TabularMedianModule::TabularMedianModule(const std::string& colName)
    : m_colName(colName) {}

std::string TabularMedianModule::getName() const {
    return "Tabular Median Module";
}

ModuleResult TabularMedianModule::apply(IData& data) {
    ModuleResult res;

    if (data.type() != DataType::Tabular) {
        res.addError(ErrorType::Error, "INVALID_DATA_TYPE",
                     "TabularMedianModule expects TabularData.");
        return res;
    }

    TabularData& table = dynamic_cast<TabularData&>(data);
    if (!table.hasColumn(m_colName)) {
        res.addError(ErrorType::Error, "COLUMN_NOT_FOUND",
                     "Column not found: " + m_colName);
        return res;
    }

    std::vector<double> values;
    for (const auto& row : table.rows()) {
        const std::string& val = row[table.columnIndex(m_colName)];
        try {
            values.push_back(std::stod(val));
        } catch (...) { continue; }
    }

    if (values.empty()) {
        res.set(m_colName + "_median", 0.0);
        res.message = "No numeric data for column: " + m_colName;
        return res;
    }

    std::sort(values.begin(), values.end());
    size_t n = values.size();
    double median = (n % 2 == 0) ? (values[n/2 -1] + values[n/2]) / 2.0 : values[n/2];

    res.set(m_colName + "_median", median);
    res.message = "Median computed for column: " + m_colName;
    return res;
}
