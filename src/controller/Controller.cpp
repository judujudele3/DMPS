#include "Controller.hpp"
#include "../engine/Engine.hpp"
#include "../ui/display/IDisplayAdapter.hpp"
#include "../ui/dialogs/ModuleConfigDialog.hpp"
#include "../moduleManager/ModuleRegistry.hpp"
#include <iostream>
#include <QMessageBox>


Controller::Controller(std::shared_ptr<Engine> engine, QObject* parent)
    : QObject(parent)
    , m_engine(engine)
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


void Controller::onEnableDisableModules()
{
    if (!m_engine) {
        QMessageBox::warning(nullptr, "Error", "Engine not initialized");
        return;
    }

    // Récupérer les modules disponibles
    auto availableModules = ModuleRegistry::getAvailableModules();

    // Récupérer les modules actuellement actifs
    auto activeModules = m_engine->getActiveModulesInfo();

    // Ouvrir le dialogue de configuration
    ModuleConfigDialog dialog(nullptr);
    dialog.setAvailableModules(availableModules);
    dialog.setActiveModules(activeModules);

    if (dialog.exec() == QDialog::Accepted) {
        // Récupérer la configuration choisie par l'utilisateur
        auto selectedModules = dialog.getSelectedModules();

        // Appliquer la configuration
        applyModuleConfiguration(selectedModules);
    }
}

void Controller::applyModuleConfiguration(const std::vector<SelectedModule>& selectedModules)
{
    if (!m_engine) {
        return;
    }

    bool success = m_engine->setActiveModules(selectedModules);

    if (!success) {
        QString errorMsg = QString::fromStdString(m_engine->getLastModuleError());
        QMessageBox::critical(nullptr, "Module Configuration Error",
                              "Failed to configure modules:\n" + errorMsg);
    }
}
