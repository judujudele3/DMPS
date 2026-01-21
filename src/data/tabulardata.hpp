#ifndef TABULARDATA_HPP
#define TABULARDATA_HPP

#include "IData.hpp"
#include <vector>
#include <string>

enum class TableFormat {
    CSV,
    XLS,
    XLSX
};

class TabularData : public IData {
public:
    explicit TabularData(TableFormat format);

    DataType type() const override;

    void setHeader(const std::vector<std::string>& header);
    const std::vector<std::string>& header() const;

    void addRow(const std::vector<std::string>& row);
    const std::vector<std::vector<std::string>>& rows() const;

    size_t rowCount() const;
    size_t columnCount() const;

    TableFormat format() const;

private:
    std::vector<std::string> m_header;
    std::vector<std::vector<std::string>> m_rows;
    TableFormat m_format;
};

#endif
