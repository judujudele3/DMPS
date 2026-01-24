#include "TabularData.hpp"

TabularData::TabularData(TableFormat format)
    : m_format(format) {}

DataType TabularData::type() const {
    return DataType::Tabular;
}

void TabularData::setHeader(const std::vector<std::string>& header) {
    m_header = header;
    m_colIndex.clear();
    for (size_t i = 0; i < header.size(); ++i) {
        m_colIndex[header[i]] = i;

    }
}


const std::vector<std::string>& TabularData::header() const {
    return m_header;
}

void TabularData::addRow(const std::vector<std::string>& row) {
    m_rows.push_back(row);
}

//const std::vector<std::vector<std::string>>& TabularData::rows() const {
  //  return m_rows;
//}

 std::vector<std::vector<std::string>>& TabularData::rows()  {
    return m_rows;
}

size_t TabularData::rowCount() const {
    return m_rows.size();
}

size_t TabularData::columnCount() const {
    return m_header.empty() ? 0 : m_header.size();
}


//
int TabularData::columnIndex(const std::string& colName) const {
    auto it = m_colIndex.find(colName);
    if (it == m_colIndex.end())
        return -1;
    return static_cast<int>(it->second);
}

bool TabularData::hasColumn(const std::string& colName) const {
    return m_colIndex.count(colName) > 0;
}

//

TableFormat TabularData::format() const {
    return m_format;
}
