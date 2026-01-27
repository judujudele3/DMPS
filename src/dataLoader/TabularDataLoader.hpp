#ifndef TABULARDATALOADER_HPP
#define TABULARDATALOADER_HPP

#include "IDataLoader.hpp"
#include "../data/TabularData.hpp"
#include <memory>
#include <string>

class TabularDataLoader : public IDataLoader {
public:
    bool supports(const std::string& extension) const override;
    std::shared_ptr<IData> load(const std::string& path) override;
    std::string getExtension(const std::string& path) const override;
};

#endif // TABULARDATALOADER_HPP
