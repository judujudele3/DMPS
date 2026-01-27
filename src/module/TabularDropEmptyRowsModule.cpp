#include "TabularDropEmptyRowsModule.hpp"
#include <vector>
#include <string>
#include <algorithm>

std::string TabularDropEmptyRowsModule::getName() const {
    return "Drop Empty Rows Module";
}

ModuleResult TabularDropEmptyRowsModule::apply(IData& data) {
    ModuleResult res;

    if (data.type() != DataType::Tabular) {
        res.addError(ErrorType::Error, "INVALID_DATA_TYPE", "TabularDropEmptyRowsModule expects TabularData.");
        return res;
    }

    TabularData& table = dynamic_cast<TabularData&>(data);

    auto& rows = table.rows();
    size_t before = rows.size();

    rows.erase(
        std::remove_if(rows.begin(), rows.end(),
                       [](const std::vector<std::string>& row) {
                           return std::all_of(row.begin(), row.end(), [](const std::string& val){ return val.empty(); });
                       }
                       ),
        rows.end()
        );

    size_t after = rows.size();
    res.message = "Dropped empty rows: " + std::to_string(before - after);
    return res;
}


std::vector<ModuleParameter> TabularDropEmptyRowsModule::getParameterDescriptors() {
    return {};
}
