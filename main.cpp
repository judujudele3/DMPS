#include "src/ui/MainWindow.hpp"
#include "src/controller/Controller.hpp"
#include "src/engine/Engine.hpp"
#include "src/ui/display/TextDisplayAdapter.hpp"
#include <memory>
#include <QApplication>
#include"src/data/ImageData.hpp"
/*
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
}*/


#include <iostream>
#include "src/module/GrayScaleModule.hpp"

int main() {
    //  Création d'une petite image 3x2 pour tester facilement
    ImageData img(3, 2, ImageFormat::JPG);

    // Remplir l'image avec des couleurs connues
    img.at(0, 0) = {255, 0, 0, 255}; // rouge
    img.at(1, 0) = {0, 255, 0, 128}; // vert semi-transparent
    img.at(2, 0) = {0, 0, 255, 255}; // bleu
    img.at(0, 1) = {100, 50, 25, 200};
    img.at(1, 1) = {200, 200, 200, 255};
    img.at(2, 1) = {10, 20, 30, 100};

    //  Appliquer le module GrayScale
    GrayScaleModule grayModule;
    ModuleResult res = grayModule.apply(img);

    // Vérifier le résultat
    std::cout << res.message << "\n";

    // Afficher les pixels après conversion
    for (int y = 0; y < img.getHeight(); ++y) {
        for (int x = 0; x < img.getWidth(); ++x) {
            Pixel& p = img.at(x, y);
            std::cout << "("
                      << (int)p.r << ","
                      << (int)p.g << ","
                      << (int)p.b << ","
                      << (int)p.a << ") ";
        }
        std::cout << "\n";
    }

    return 0;
}

