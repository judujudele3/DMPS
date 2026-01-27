#include "TabularNormalizeModule.hpp"
#include <algorithm>
#include<limits>

std::string TabularNormalizeModule::getName() const {
    return "Normalize Module";
}

ModuleResult TabularNormalizeModule::apply(IData& data) {
    ModuleResult res;
    if (data.type() != DataType::Tabular) {
        res.addError(ErrorType::Error, "INVALID_DATA_TYPE", "TabularNormalizeModule expects TabularData.");
        return res;
    }

    TabularData& table = dynamic_cast<TabularData&>(data);
    for (const auto& colName : table.header()) {
        int idx = table.columnIndex(colName);
        double minVal = std::numeric_limits<double>::max();
        double maxVal = std::numeric_limits<double>::lowest();

        for (auto& row : table.rows()) {
            if (!row[idx].empty()) {
                double val = std::stod(row[idx]);
                minVal = std::min(minVal, val);
                maxVal = std::max(maxVal, val);
            }
        }

        for (auto& row : table.rows()) {
            if (!row[idx].empty()) {
                double val = std::stod(row[idx]);
                row[idx] = std::to_string((val - minVal) / (maxVal - minVal));
            }
        }
    }

    res.message = "Normalized numeric columns.";
    return res;
}


std::vector<ModuleParameter> TabularNormalizeModule::getParameterDescriptors() {
    return {
    };
}
