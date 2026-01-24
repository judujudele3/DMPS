#include "TabularStdModule.hpp"
#include "TabularMeanModule.hpp"

TabularStdModule::TabularStdModule(const std::string& colName)
    : m_colName(colName) {}

std::string TabularStdModule::getName() const {
    return "Tabular Std Module";
}

ModuleResult TabularStdModule::apply(IData& data) {
    ModuleResult res;

    if (data.type() != DataType::Tabular) {
        res.addError(ErrorType::Error, "INVALID_DATA_TYPE",
                     "TabularStdModule expects TabularData.");
        return res;
    }

    TabularData& table = dynamic_cast<TabularData&>(data);

    if (!table.hasColumn(m_colName)) {
        res.addError(ErrorType::Error, "COLUMN_NOT_FOUND",
                     "Column not found: " + m_colName);
        return res;
    }

    TabularMeanModule meanModule(m_colName);
    ModuleResult meanRes = meanModule.apply(data);
    double mean = meanRes.get<double>("mean");

    double variance = 0.0;
    int validCount = 0;

    for (const auto& row : table.rows()) {
        const std::string& val = row[table.columnIndex(m_colName)];
        try {
            double v = std::stod(val);
            variance += (v - mean) * (v - mean);
            validCount++;
        } catch (...) {
            continue;
        }
    }

    double stddev = (validCount > 0) ? std::sqrt(variance / validCount) : 0.0;

    res.set("std", stddev);
    res.message = "Standard deviation computed for column: " + m_colName;
    return res;
}
