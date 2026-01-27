#ifndef TABULARDATA_HPP
#define TABULARDATA_HPP

#include "IData.hpp"
#include <vector>
#include <string>
#include<unordered_map>

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
    // on a supprimé const pour que on peut travailler sur le tabulardata
    std::vector<std::vector<std::string>>& rows() ;
    size_t rowCount() const;
    size_t columnCount() const;

    int columnIndex(const std::string& colName) const;
    bool hasColumn(const std::string& colName) const ;


    TableFormat format() const;

private:
    // définir la text "columns"
    std::vector<std::string> m_header;
    //index de colones
    std::unordered_map<std::string,size_t> m_colIndex;
    std::vector<std::vector<std::string>> m_rows;
    TableFormat m_format;
};

#endif
