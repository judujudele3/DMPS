#ifndef DATALOADERMANAGER_HPP
#define DATALOADERMANAGER_HPP

#pragma once
#include <string>
#include <memory>
#include <vector>
#include "../data/IData.hpp"

class IDataLoader;

class DataLoaderManager
{
public:
    void addLoader(std::shared_ptr<IDataLoader> loader);
    std::shared_ptr<IData> loadData(const std::string& path);

private:
    std::vector<std::shared_ptr<IDataLoader>> m_loaders;
};


#endif // DATALOADERMANAGER_HPP
