#include "TabularMeanModule.hpp"
#include "TabularSumModule.hpp"
#include "TabularCountModule.hpp"

TabularMeanModule::TabularMeanModule(const std::string& colName)
    : m_colName(colName) {}

std::string TabularMeanModule::getName() const {
    return "Tabular Mean Module";
}

ModuleResult TabularMeanModule::apply(IData& data) {
    ModuleResult res;

    // vérifier type
    if (data.type() != DataType::Tabular) {
        res.addError(ErrorType::Error, "INVALID_DATA_TYPE",
                     "TabularMeanModule expects TabularData.");
        return res;
    }

    TabularData& table = dynamic_cast<TabularData&>(data);

    if (!table.hasColumn(m_colName)) {
        res.addError(ErrorType::Error, "COLUMN_NOT_FOUND",
                     "Column not found: " + m_colName);
        return res;
    }

    // Réutiliser Sum et Count
    TabularSumModule sumModule(m_colName);
    ModuleResult sumRes = sumModule.apply(data);
    double sum = sumRes.get<double>(m_colName + "_sum");

    TabularCountModule countModule(m_colName);
    ModuleResult countRes = countModule.apply(data);
    int count = countRes.get<int>(m_colName + "_count");

    double mean = (count != 0) ? sum / count : 0.0;

    res.set(m_colName + "_mean", mean);
    res.message = "Mean computed for column: " + m_colName;
    return res;
}
