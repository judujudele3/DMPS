#include "MainWindow.hpp"
#include "ui_MainWindow.h"
#include <QFileDialog>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);  // Initialise l'interface définie dans MainWindow.ui
    // connexion des actions définies dans le .ui
    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::onQuit);
    connect(ui->actionApply_Modules, &QAction::triggered, this, &MainWindow::onApplyModule);
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::onAbout);
    setupDockWidgets();

}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::setController(Controller* controller)
{
    m_controller = controller;

    connect(this, &MainWindow::enableDisableModulesRequested,
            m_controller, &Controller::onEnableDisableModules);
}

void MainWindow::on_actionOpen_triggered()
{
    QString filePath = QFileDialog::getOpenFileName(
        this,
        tr("Open file"),
        "",
        tr("Text files (*.txt);;All files (*.*)")
        );

    if (filePath.isEmpty())
        return;


    if (m_controller)
        m_controller->openFile(filePath.toStdString());

    if (!m_controller) {
        std::cout << "[MainWindow] ERROR: m_controller is null !" << std::endl;
        return;
    }

    m_controller->displayCurrentData(ui->stackedWidget);


}

void MainWindow::onQuit() {
    close();  // ferme la fenêtre principale
}

void MainWindow::onApplyModule() {
    if (m_controller) {
        m_controller->onApplyModules(this); // Passer 'this' comme parent
    } else {
        QMessageBox::warning(this, "Error", "Controller not initialized");
    }
}

void MainWindow::onAbout() {
    QMessageBox::about(this, "À propos",
                       "DMPS — Data & Media Processing Studio\nVersion prototype");
}

void MainWindow::on_actionEnable_Disable_triggered()
{
    emit enableDisableModulesRequested();
}


void MainWindow::setupDockWidgets()
{
    // Create Results Explorer (left side)
    resultsExplorer_ = new ResultsExplorerWidget(this);
    addDockWidget(Qt::LeftDockWidgetArea, resultsExplorer_);
    resultsExplorer_->hide(); // Hidden initially

    // Create Messages Log (bottom)
    messagesLog_ = new MessagesLogWidget(this);
    addDockWidget(Qt::BottomDockWidgetArea, messagesLog_);
    messagesLog_->hide(); // Hidden initially

    // Add to View menu (optional)
    QMenu* viewMenu = menuBar()->addMenu("View");
    viewMenu->addAction(resultsExplorer_->toggleViewAction());
    viewMenu->addAction(messagesLog_->toggleViewAction());
}

void MainWindow::displayModuleResults(const std::vector<ModuleExecutionResult>& results)
{
    // Show panels if hidden
    if (resultsExplorer_->isHidden()) {
        resultsExplorer_->show();
    }
    if (messagesLog_->isHidden()) {
        messagesLog_->show();
    }

    // Display results
    resultsExplorer_->displayResults(results);
    messagesLog_->logResults(results);
}
