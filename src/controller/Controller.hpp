#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#pragma once
#include "../moduleManager/ModuleManager.hpp"
#include "../ui/helpers/SaveFileDialogHelper.hpp"
#include <QFileDialog>
#include <string>
#include <memory>
#include <vector>
#include<QObject>

class IData; // forward declaration
class Engine;
class IDisplayAdapter;
class QWidget;
class MainWindow;

class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(std::shared_ptr<Engine> engine, QObject* parent = nullptr);
    // ouverture de fichier : le controller reçoit le path et demande à l'engine de charger
    void openFile(const std::string &path);
    void onApplyModules(MainWindow* mainWindow);
    void addDisplayAdapter(std::shared_ptr<IDisplayAdapter> adapter);
    void displayCurrentData(QWidget* container);
    void saveDataAs(QWidget* parentWidget);
    void closeFile();
    bool hasData() const;

public slots:
    void onEnableDisableModules();

private:
    std::shared_ptr<Engine> m_engine;
    std::vector<std::shared_ptr<IDisplayAdapter>> m_displayAdapters;
    void applyModuleConfiguration(const std::vector<SelectedModule>& selectedModules);
};


#endif // CONTROLLER_HPP
