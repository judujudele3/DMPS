#include "src/ui/MainWindow.hpp"
#include "src/controller/Controller.hpp"
#include "src/engine/Engine.hpp"
#include "src/ui/display/TextDisplayAdapter.hpp"
#include <memory>
#include <QApplication>
#include"src/data/ImageData.hpp"
#include <cstdint>
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
}
#include "src/dataloader/ImageDataLoader.hpp"
#include <iostream>
#include <memory>

int main() {
    ImageDataLoader loader;

    // 🔹 Chemin vers l'image de test
    std::string path = "C:/Users/user/Desktop/dossier_c/DMPS/src/test_data/test.jpeg"; // Remplace par ton image

    // Vérifier si le loader supporte l'extension
    std::string ext = loader.getExtension(path);
    if (!loader.supports(ext)) {
        std::cerr << "Format non supporté : " << ext << std::endl;
        return 1;
    }

    // Charger l'image
    std::shared_ptr<IData> data = loader.load(path);
    if (!data) {
        std::cerr << "Échec du chargement de l'image." << std::endl;
        return 1;
    }

    // Vérifier que c'est bien un ImageData
    auto* image = dynamic_cast<ImageData*>(data.get());
    if (!image) {
        std::cerr << "Erreur: IData n'est pas une ImageData." << std::endl;
        return 1;
    }

    // Afficher des informations basiques
    std::cout << "Image loaded: " << image->getWidth()
              << "x" << image->getHeight()  << std::endl;


    // Afficher les 5 premiers pixels pour vérification
    std::cout << "Premiers pixels: ";
    for (int i = 0; i < std::min(5, (int)image->pixels().size()); ++i) {
        Pixel& p = image->pixels()[i];
        std::cout << "(" << (int)p.r << "," << (int)p.g << "," << (int)p.b << "," << (int)p.a << ") ";
    }
    std::cout << std::endl;
    std::cout<< "the pixel (5,5): \n"<<"a: "<<(int)image->at(5,5).a<<"\nb: "<<(int)image->at(5,5).b<<"\ng: "<<(int)image->at(5,5).g<<"\nr: "<<(int)image->at(5,5).r<<std::endl;
    return 0;
}
*/


#include "src/dataLoader/TabularDataLoader.hpp"
#include <iostream>
#include <memory>

int main() {
    TabularDataLoader loader;

    // 🔹 Chemin vers le fichier CSV de test
    std::string path = "C:/Users/user/Desktop/dossier_c/DMPS/src/test_data/sales.csv"; // remplace par ton fichier

    // Vérifier si le loader supporte l'extension
    std::string ext = path.substr(path.find_last_of('.') + 1);
    if (!loader.supports(ext)) {
        std::cerr << "Format non supporté : " << ext << std::endl;
        return 1;
    }

    // Charger le CSV
    std::shared_ptr<IData> data = loader.load(path);
    if (!data) {
        std::cerr << "Échec du chargement du CSV." << std::endl;
        return 1;
    }

    // Vérifier que c'est bien un TabularData
    auto* table = dynamic_cast<TabularData*>(data.get());
    if (!table) {
        std::cerr << "Erreur: IData n'est pas un TabularData." << std::endl;
        return 1;
    }

    // Afficher le header
    std::cout << "Header: ";
    for (const auto& col : table->header()) std::cout << col << " ";
    std::cout << std::endl;

    // Afficher les 5 premières lignes
    std::cout << "Premières lignes:" << std::endl;
    int n = std::min(5, (int)table->rowCount());
    for (int i = 0; i < n; ++i) {
        const auto& row = table->rows()[i];
        for (const auto& cell : row) std::cout << cell << " ";
        std::cout << std::endl;
    }

    return 0;
}


