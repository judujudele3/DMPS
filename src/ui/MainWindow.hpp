#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QMessageBox>
#include "../controller/Controller.hpp"

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

private:
    Ui::MainWindow *ui;
    Controller* m_controller = nullptr;

private slots:
    void on_actionOpen_triggered();
    void onQuit();           // appelé quand on clique sur "Quitter"
    void onApplyModule();    // appelé quand on clique sur "Appliquer"
    void onAbout();          // appelé quand on clique sur "À propos"

};

#endif // MAINWINDOW_HPP
