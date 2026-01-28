#include "DataLoaderManager.hpp"
#include "../dataLoader/IDataLoader.hpp"
#include <iostream>
#include <filesystem>

void DataLoaderManager::addLoader(std::shared_ptr<IDataLoader> loader)
{
    m_loaders.push_back(loader);
}

std::shared_ptr<IData> DataLoaderManager::loadData(const std::string& path)
{
    std::filesystem::path fsPath(path);
    std::string ext = fsPath.extension().string();

    for (auto& loader : m_loaders)
    {
        if (loader->supports(ext))
            return loader->load(path);
    }

    std::cerr << "No loader for: " << ext << std::endl;
    return nullptr;
}
