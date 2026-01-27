#include "src/ui/MainWindow.hpp"
#include "src/controller/Controller.hpp"
#include "src/engine/Engine.hpp"
#include "src/ui/display/TextDisplayAdapter.hpp"
#include <memory>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Créer l'engine
    std::shared_ptr<Engine> engine = std::make_shared<Engine>();

    // Créer le controller (doit hériter de QObject maintenant)
    Controller* controller = new Controller(engine, &a);  // ← parent = &a pour gestion mémoire Qt

    // Ajouter les display adapters
    controller->addDisplayAdapter(std::make_shared<TextDisplayAdapter>());

    // Créer la fenêtre principale
    MainWindow w;
    w.setController(controller);

    w.show();
    return a.exec();
}



