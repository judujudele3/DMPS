#include "Controller.hpp"
#include "../engine/Engine.hpp"
#include "../ui/display/IDisplayAdapter.hpp"
#include "../ui/dialogs/ModuleConfigDialog.hpp"
#include "../moduleManager/ModuleRegistry.hpp"
#include "../ui/MainWindow.hpp"
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


void Controller::onApplyModules(QWidget* parentWidget)
{
    if (!m_engine) {
        QMessageBox::warning(parentWidget, "Engine Error", "Engine is not initialized.");
        return;
    }

    // Check if data is loaded
    if (!m_engine->getData()) {
        QMessageBox::warning(parentWidget, "No Data",
                             "Please load a file before applying modules.");
        return;
    }

    // Check if modules are active
    if (m_engine->getModuleCount() == 0) {
        QMessageBox::information(parentWidget, "No Modules",
                                 "No modules are currently active.\n\n"
                                 "Please enable modules via Modules → Enable/Disable first.");
        return;
    }

    // Execute modules
    auto results = m_engine->applyModules();

    // Cast parentWidget to MainWindow to call displayModuleResults
    MainWindow* mainWindow = qobject_cast<MainWindow*>(parentWidget);
    if (mainWindow) {
        mainWindow->displayModuleResults(results);
    }

    // Count successes and errors
    int successCount = 0;
    int errorCount = 0;

    for (const auto& result : results) {
        if (result.isSuccess() && !result.hasErrors()) {
            successCount++;
        } else {
            errorCount++;
        }
    }

    // Show summary message if there were errors
    if (errorCount > 0) {
        QString errorSummary = QString("%1 module%2 failed:\n\n")
        .arg(errorCount)
            .arg(errorCount > 1 ? "s" : "");

        for (const auto& result : results) {
            if (result.hasErrors()) {
                errorSummary += "• " + QString::fromStdString(result.displayName) + "\n";
            }
        }

        errorSummary += "\nCheck the Messages panel for details.";

        QMessageBox::warning(parentWidget, "Execution Errors", errorSummary);
    }
}
