#include "TabularFilterNumericColumnsModule.hpp"
#include <cctype>

std::string TabularFilterNumericColumnsModule::getName() const {
    return "Filter Numeric Columns Module";
}

ModuleResult TabularFilterNumericColumnsModule::apply(IData& data) {
    ModuleResult res;

    if (data.type() != DataType::Tabular) {
        res.addError(ErrorType::Error,
                     "INVALID_DATA_TYPE",
                     "TabularFilterNumericColumnsModule expects TabularData.");
        return res;
    }

    TabularData& table = dynamic_cast<TabularData&>(data);

    const auto& header = table.header();
    auto& rows = table.rows();   // 🔴 NON const (important)

    std::vector<std::string> newHeader;
    std::vector<int> numericIndices;

    for (size_t c = 0; c < header.size(); ++c) {
        bool isNumeric = true;

        for (const auto& row : rows) {
            const std::string& val = row[c];
            if (!val.empty()) {
                try {
                    std::stod(val);
                } catch (...) {
                    isNumeric = false;
                    break;
                }
            }
        }

        if (isNumeric) {
            newHeader.push_back(header[c]);
            numericIndices.push_back(static_cast<int>(c));
        }
    }

    std::vector<std::vector<std::string>> newRows;
    newRows.reserve(rows.size());

    for (const auto& row : rows) {
        std::vector<std::string> newRow;
        newRow.reserve(numericIndices.size());

        for (int idx : numericIndices) {
            newRow.push_back(row[idx]);
        }
        newRows.push_back(std::move(newRow));
    }

    table.setHeader(newHeader);
    rows = std::move(newRows);

    res.message = "Kept " + std::to_string(newHeader.size()) + " numeric columns.";
    return res;
}

std::vector<ModuleParameter> TabularFilterNumericColumnsModule::getParameterDescriptors() {
    return {};
}
