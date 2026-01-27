#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QMessageBox>
#include "../controller/Controller.hpp"
#include "../core/ModuleExecutionResult.hpp"
#include "widgets/ResultsExplorerWidget.hpp"
#include "widgets/MessagesLogWidget.hpp"
#include <vector>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setController(Controller* controller);
    void displayModuleResults(const std::vector<ModuleExecutionResult>& results);

private:
    Ui::MainWindow *ui;
    Controller* m_controller = nullptr;
    ResultsExplorerWidget* resultsExplorer_;
    MessagesLogWidget* messagesLog_;

    void setupDockWidgets();


signals:
    void enableDisableModulesRequested();

private slots:
    void on_actionOpen_triggered();
    void onQuit();           // appelé quand on clique sur "Quitter"
    void onApplyModule();    // appelé quand on clique sur "Appliquer"
    void onAbout();          // appelé quand on clique sur "À propos"
    void on_actionEnable_Disable_triggered(); // appelé quand on clique sur "Enable/Disable"

};

#endif // MAINWINDOW_HPP
