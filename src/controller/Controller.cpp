#include "Controller.hpp"
#include "../engine/Engine.hpp"
#include "../ui/display/IDisplayAdapter.hpp"
#include <iostream>


Controller::Controller(std::shared_ptr<Engine> engine)
    : m_engine(engine)
{
}

void Controller::openFile(const std::string &path)
{
    if (path.empty())
        return;

    std::cout << "[Controller] Ouverture du fichier : " << path << std::endl;

    auto data = m_engine->loadData(path);
    if (!data)
    {
        std::cout << "[Controller] Échec du chargement." << std::endl;
        return;
    }

    std::cout << "[Controller] Données prêtes pour traitement." << std::endl;
}

void Controller::addDisplayAdapter(std::shared_ptr<IDisplayAdapter> adapter)
{
    m_displayAdapters.push_back(adapter);
}


void Controller::displayCurrentData(QWidget* container)
{
    auto data = m_engine->getData();
    if (!data)
        return;

    for (auto& adapter : m_displayAdapters)
    {
        if (adapter->canDisplay(*data))
        {
            adapter->display(*data, container);
            return;
        }
    }
}
