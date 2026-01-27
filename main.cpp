#include "src/ui/MainWindow.hpp"
#include "src/controller/Controller.hpp"
#include "src/engine/Engine.hpp"
#include "src/ui/display/TextDisplayAdapter.hpp"
#include "src/ui/display/ImageDisplayAdapter.hpp"     // ← NOUVEAU
#include "src/ui/display/TabularDisplayAdapter.hpp"
#include <memory>
#include <QApplication>
#include"src/data/ImageData.hpp"
#include <cstdint>
#include <QLoggingCategory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QLoggingCategory::setFilterRules("*.debug=true");

    // Créer l'engine
    std::shared_ptr<Engine> engine = std::make_shared<Engine>();

    // Créer le controller (doit hériter de QObject maintenant)
    Controller* controller = new Controller(engine, &a);  // ← parent = &a pour gestion mémoire Qt

    // Ajouter les display adapters
    controller->addDisplayAdapter(std::make_shared<TextDisplayAdapter>());
    controller->addDisplayAdapter(std::make_shared<ImageDisplayAdapter>());
    controller->addDisplayAdapter(std::make_shared<TabularDisplayAdapter>());

    // Créer la fenêtre principale
    MainWindow w;
    w.setController(controller);

    w.show();
    return a.exec();

}


