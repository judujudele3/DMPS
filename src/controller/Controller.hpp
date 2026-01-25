#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#pragma once

#include <string>
#include <memory>
#include <vector>

class IData; // forward declaration
class Engine;
class IDisplayAdapter;
class QWidget;

class Controller
{
public:
    Controller(std::shared_ptr<Engine> engine);

    // ouverture de fichier : le controller reçoit le path et demande à l'engine de charger
    void openFile(const std::string &path);

    void addDisplayAdapter(std::shared_ptr<IDisplayAdapter> adapter);
    void displayCurrentData(QWidget* container);

private:
    std::shared_ptr<Engine> m_engine;
    std::vector<std::shared_ptr<IDisplayAdapter>> m_displayAdapters;
};


#endif // CONTROLLER_HPP
