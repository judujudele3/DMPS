#include "TabularIsNullModule.hpp"

TabularIsNullModule::TabularIsNullModule(const std::string& colName)
    : m_colName(colName) {}

std::string TabularIsNullModule::getName() const {
    return "Tabular IsNull Module";
}

ModuleResult TabularIsNullModule::apply(IData& data) {
    ModuleResult res;

    if (data.type() != DataType::Tabular) {
        res.addError(ErrorType::Error, "INVALID_DATA_TYPE",
                     "TabularIsNullModule expects TabularData.");
        return res;
    }

    TabularData& table = dynamic_cast<TabularData&>(data);

    if (!table.hasColumn(m_colName)) {
        res.addError(ErrorType::Error, "COLUMN_NOT_FOUND",
                     "Column not found: " + m_colName);
        return res;
    }

    int nullCount = 0;
    for (const auto& row : table.rows()) {
        const std::string& val = row[table.columnIndex(m_colName)];
        if (val.empty()) nullCount++;
    }

    res.set("isnull", nullCount);
    res.message = "Null values counted for column: " + m_colName;
    return res;
}


std::vector<ModuleParameter> TabularIsNullModule::getParameterDescriptors() {
    return {
        {"colName", "string", std::string(""), "Nom de la colonne à vérifier pour les valeurs nulles"}
    };
}
