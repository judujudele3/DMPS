#include "TabularData.hpp"

TabularData::TabularData(TableFormat format)
    : m_format(format) {}

DataType TabularData::type() const {
    return DataType::Tabular;
}

void TabularData::setHeader(const std::vector<std::string>& header) {
    m_header = header;
}

const std::vector<std::string>& TabularData::header() const {
    return m_header;
}

void TabularData::addRow(const std::vector<std::string>& row) {
    m_rows.push_back(row);
}

const std::vector<std::vector<std::string>>& TabularData::rows() const {
    return m_rows;
}

size_t TabularData::rowCount() const {
    return m_rows.size();
}

size_t TabularData::columnCount() const {
    return m_header.empty() ? 0 : m_header.size();
}

TableFormat TabularData::format() const {
    return m_format;
}
