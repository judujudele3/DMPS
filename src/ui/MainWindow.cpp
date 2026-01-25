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

}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::setController(Controller* controller)
{
    m_controller = controller;
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

    m_controller->displayCurrentData(ui->stackedWidget);

}

void MainWindow::onQuit() {
    close();  // ferme la fenêtre principale
}

void MainWindow::onApplyModule() {
    QMessageBox::information(this, "Appliquer", "Appliquer module (à implémenter)");
}

void MainWindow::onAbout() {
    QMessageBox::about(this, "À propos",
                       "DMPS — Data & Media Processing Studio\nVersion prototype");
}
