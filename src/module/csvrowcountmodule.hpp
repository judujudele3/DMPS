#ifndef CSVROWCOUNTMODULE_HPP
#define CSVROWCOUNTMODULE_HPP

#include "IModule.hpp"
#include "../data/CsvData.hpp"

class CsvRowCountModule : public IModule {
public:
    std::string getName() const override;
    ModuleResult apply(IData& data) override;
};

#endif
