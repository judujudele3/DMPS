#include "TabularDataSaver.hpp"

bool TabularDataSaver::save(const IData& data, const std::string& path) {
    const TabularData* tabData = dynamic_cast<const TabularData*>(&data);
    if (!tabData) {
        std::cerr << "Error: Provided data is not TabularData.\n";
        return false;
    }

    std::ofstream out(path);
    if (!out) {
        std::cerr << "Error: Cannot open file " << path << " for writing.\n";
        return false;
    }

    std::string ext = path.substr(path.find_last_of('.') + 1);
    if (ext == "csv" || ext == "xlsx" ) {
        // Write CSV header
        const auto& header = tabData->header();
        for (size_t i = 0; i < header.size(); ++i) {
            out << header[i];
            if (i + 1 < header.size()) out << ",";
        }
        out << "\n";

        // Write rows
        for (auto& row : tabData->rows()) {
            for (size_t i = 0; i < row.size(); ++i) {
                out << row[i];
                if (i + 1 < row.size()) out << ",";
            }
            out << "\n";
        }
    }
    else {
        std::cerr << "Error: Unsupported extension ." << ext << "\n";
        return false;
    }

    out.close();
    return true;
}
