#include "TabularFillMissingValuesModule.hpp"

TabularFillMissingValuesModule::TabularFillMissingValuesModule(const std::string& column, const std::string& fillValue)
    : m_column(column), m_fillValue(fillValue) {}

std::string TabularFillMissingValuesModule::getName() const {
    return "Fill Missing Values Module";
}

ModuleResult TabularFillMissingValuesModule::apply(IData& data) {
    ModuleResult res;
    if (data.type() != DataType::Tabular) {
        res.addError(ErrorType::Error, "INVALID_DATA_TYPE", "TabularFillMissingModule expects TabularData.");
        return res;
    }

    TabularData& table = dynamic_cast<TabularData&>(data);
    int idx = table.columnIndex(m_column);
    if (idx == -1) {
        res.addError(ErrorType::Error, "COLUMN_NOT_FOUND", "Column not found: " + m_column);
        return res;
    }

    size_t filled = 0;
    for (auto& row : table.rows()) {
        if (row[idx].empty()) {
            row[idx] = m_fillValue;
            filled++;
        }
    }
    res.message = "Filled " + std::to_string(filled) + " missing values in column: " + m_column;
    return res;
}

std::vector<ModuleParameter> TabularFillMissingValuesModule::getParameterDescriptors() {
    return {
        {"column", "string", std::string(""), "Nom de la colonne à remplir"},
        {"fillValue", "string", std::string("0"), "Valeur de remplissage pour les cases vides"}
    };
}
