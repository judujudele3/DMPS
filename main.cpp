#include "src/ui/MainWindow.hpp"
#include "src/controller/Controller.hpp"
#include "src/engine/Engine.hpp"
#include "src/ui/display/TextDisplayAdapter.hpp"

#include <memory>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    std::shared_ptr<Engine> engine = std::make_shared<Engine>();
    Controller controller(engine);
    controller.addDisplayAdapter(
        std::make_shared<TextDisplayAdapter>()
        );

    MainWindow w;
    w.setController(&controller);

    w.show();
    return a.exec();
}
