
#include "Controller.hpp"
#include "../engine/Engine.hpp"
#include "../ui/display/IDisplayAdapter.hpp"
#include "../ui/dialogs/ModuleConfigDialog.hpp"
#include "../moduleManager/ModuleRegistry.hpp"
#include "../ui/display/ImageDisplayAdapter.hpp"
#include "../ui/display/TabularDisplayAdapter.hpp"
#include "../ui/display/TextDisplayAdapter.hpp"
#include "../ui/MainWindow.hpp"
#include "../ui/helpers/SaveFileDialogHelper.hpp"
#include <iostream>
#include <QMessageBox>
#include <QStackedWidget>
#include <QFileDialog>




Controller::Controller(std::shared_ptr<Engine> engine, QObject* parent)
    : QObject(parent)
    , m_engine(engine)
{
    // Image
    addDisplayAdapter(std::make_shared<ImageDisplayAdapter>());

    // Tabular
    addDisplayAdapter(std::make_shared<TabularDisplayAdapter>());

    // Texte
    addDisplayAdapter(std::make_shared<TextDisplayAdapter>());
}



void Controller::openFile(const std::string &path)
{
    if (path.empty())
        return;

    std::cout << "[Controller] Data ready for processing : " << path << std::endl;

    auto data = m_engine->loadData(path);
    if (!data)
    {
        std::cout << "[Controller] Loading failed" << std::endl;
        return;
    }

    std::cout << "[Controller] Data ready for processing." << std::endl;
}



void Controller::addDisplayAdapter(std::shared_ptr<IDisplayAdapter> adapter)
{
    m_displayAdapters.push_back(adapter);
}



void Controller::displayCurrentData(QWidget* container)
{

    auto data = m_engine->getData();

    std::cout << "[DEBUG] displayCurrentData called" << std::endl;
    if (!data) {
        std::cout << "[DEBUG] No data loaded!" << std::endl;
        return;
    }
    std::cout << "[DEBUG] Data type: " << static_cast<int>(data->type()) << std::endl;


    int i = 0;

    for (auto& adapter : m_displayAdapters) {
        std::cout << "[Controller] Test adapter " << i++ << std::endl;

        bool canDisplay = adapter->canDisplay(*data);
        std::cout << "[Controller]   -> canDisplay: " << canDisplay << std::endl;

        if (canDisplay) {
            std::cout << "[Controller] Display using adapter " << (i-1) << std::endl;
            adapter->display(*data, container);
            return;
        }
    }


    std::cout << "[Controller] NO COMPATIBLE ADAPTER FOUND!" << std::endl;
}


void Controller::onEnableDisableModules()
{
    if (!m_engine) {
        QMessageBox::warning(nullptr, "Error", "Engine not initialized");
        return;
    }

    // Retrieve available modules
    auto availableModules = ModuleRegistry::getAvailableModules();

    // Retrieve currently active modules
    auto activeModules = m_engine->getActiveModulesInfo();

    // Opening configuration dialog
    ModuleConfigDialog dialog(nullptr);
    dialog.setAvailableModules(availableModules);
    dialog.setActiveModules(activeModules);

    if (dialog.exec() == QDialog::Accepted) {
        // Retrieve user-selected configuration
        auto selectedModules = dialog.getSelectedModules();

        // Apply settings
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


void Controller::onApplyModules(MainWindow* mainWindow)
{
    if (!m_engine) {
        QMessageBox::warning(mainWindow, "Engine Error", "Engine is not initialized.");
        return;
    }

    // Check if data is loaded
    if (!m_engine->getData()) {
        QMessageBox::warning(mainWindow, "No Data",
                             "Please load a file before applying modules.");
        return;
    }

    // Check if modules are active
    if (m_engine->getModuleCount() == 0) {
        QMessageBox::information(mainWindow, "No Modules",
                                 "No modules are currently active.\n\n"
                                 "Please enable modules via Modules → Enable/Disable first.");
        return;
    }

    // Execute modules
    std::cout << "[Controller] Applying modules..." << std::endl;
    auto results = m_engine->applyModules();

    //  RAFRAÎCHIR L'AFFICHAGE DE LA DONNÉE
    if (mainWindow) {
        // Récupérer le container widget (stackedWidget dans ton cas)
        QStackedWidget* container = mainWindow->findChild<QStackedWidget*>("stackedWidget");
        if (container) {
            displayCurrentData(container);
        }
    }

    // Display results
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

        QMessageBox::warning(mainWindow, "Execution Errors", errorSummary);
    }
}

void Controller::saveDataAs(QWidget* parentWidget)
{
    if (!m_engine) {
        QMessageBox::warning(parentWidget, "Engine Error", "Engine is not initialized.");
        return;
    }

    // Check if data is loaded
    auto data = m_engine->getData();
    if (!data) {
        QMessageBox::warning(parentWidget, "No Data",
                             "No data to save. Please load a file first.");
        return;
    }

    // Generate file filter based on data type
    QString filter = SaveFileDialogHelper::generateFilter(*data, m_engine->getSaveManager());
    QString defaultExt = SaveFileDialogHelper::getDefaultExtension(*data);

    // Show Save File Dialog
    QString filePath = QFileDialog::getSaveFileName(
        parentWidget,
        tr("Save File As"),
        "",  // Default directory (empty = current)
        filter
        );

    if (filePath.isEmpty()) {
        // User cancelled
        return;
    }

    // Save the data
    bool success = m_engine->saveData(filePath.toStdString());

    if (success) {
        QMessageBox::information(parentWidget, "Success",
                                 "File saved successfully to:\n" + filePath);
    } else {
        QMessageBox::critical(parentWidget, "Save Error",
                              "Failed to save file to:\n" + filePath +
                                  "\n\nCheck console for details.");
    }
}

void Controller::closeFile()
{
    std::cout << "[Controller] Fermeture du fichier..." << std::endl;

    // Réinitialiser les données dans l'engine
    if (m_engine) {
        m_engine->clearData();
    }

    std::cout << "[Controller] Fichier fermé." << std::endl;
}

bool Controller::hasData() const
{
    return m_engine && m_engine->getData() != nullptr;
}
