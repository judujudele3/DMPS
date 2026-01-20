#include "CsvRowCountModule.hpp"

std::string CsvRowCountModule::getName() const {
    return "CSV Row Count Module";
}

ModuleResult CsvRowCountModule::apply(IData& data) {
    ModuleResult res;

    if (data.type() != DataType::Csv) {
        res.addError(ErrorType::Error,
                     "INVALID_DATA_TYPE",
                     "CsvRowCountModule expects CsvData.");
        return res;
    }

    CsvData& csv = dynamic_cast<CsvData&>(data);
    size_t rows = csv.getRows().size();

    res.message = "Number of rows = " + std::to_string(rows);
    return res;
}

