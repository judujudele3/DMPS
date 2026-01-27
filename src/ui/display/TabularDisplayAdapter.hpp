#ifndef TABULARDISPLAYADAPTER_HPP
#define TABULARDISPLAYADAPTER_HPP

#include "IDisplayAdapter.hpp"
#include "../../data/TabularData.hpp"
#include <memory>

class QTableWidget;

class TabularDisplayAdapter : public IDisplayAdapter
{
public:
    TabularDisplayAdapter() = default;
    ~TabularDisplayAdapter() override = default;

    bool canDisplay(const IData& data) const override;
    void display(const IData& data, QWidget* container) override;

private:
    QTableWidget* createTableWidget(const TabularData& tabularData, QWidget* parent);
    void populateTable(QTableWidget* table, const TabularData& tabularData);
};

#endif // TABULARDISPLAYADAPTER_HPP
