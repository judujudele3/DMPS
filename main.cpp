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

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

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


/*
#include "src/DataSaveManager/DataSaveManager.hpp"
#include "src/DataSaver/TextDataSaver.hpp"
#include "src/DataSaver/TabularDataSaver.hpp"
#include "src/DataSaver/ImageDataSaver.hpp"

#include "src/data/TextData.hpp"
#include "src/data/TabularData.hpp"
#include "src/data/ImageData.hpp"

#include <memory>
#include <iostream>
#include <filesystem>

int main() {
    DataSaveManager saveManager;

    // 1️⃣ Register savers
    saveManager.registerSaver(std::make_shared<TextDataSaver>());
    saveManager.registerSaver(std::make_shared<TabularDataSaver>());
    saveManager.registerSaver(std::make_shared<ImageDataSaver>());

    // ----------------------------
    // Test 1: TextData
    // ----------------------------
    auto text = std::make_shared<TextData>("This is a sample text for testing.");
    std::string textPath = "C:/Users/user/Desktop/text_sample.txt";

    if (saveManager.saveData(*text, textPath)) {
        std::cout << "TextData saved successfully to " << textPath << std::endl;
    } else {
        std::cerr << "Failed to save TextData!" << std::endl;
    }

    // ----------------------------
    // Test 2: TabularData
    // ----------------------------
    TabularData tabData(TableFormat::CSV);
    tabData.setHeader({"Name", "Age", "Score"});
    tabData.addRow({"Alice", "23", "88"});
    tabData.addRow({"Bob", "25", "92"});

    std::string tabPath = "C:/Users/user/Desktop/tabular_sample.csv";

    if (saveManager.saveData(tabData, tabPath)) {
        std::cout << "TabularData saved successfully to " << tabPath << std::endl;
    } else {
        std::cerr << "Failed to save TabularData!" << std::endl;
    }

    // ----------------------------
    // Test 3: ImageData
    // ----------------------------
    ImageData img(2, 2, ImageFormat::PNG);
    img.at(0,0) = {255,0,0,255};
    img.at(1,0) = {0,255,0,255};
    img.at(0,1) = {0,0,255,255};
    img.at(1,1) = {255,255,0,255};

    std::string imgPath = "C:/Users/user/Desktop/image_sample.png";

    if (saveManager.saveData(img, imgPath)) {
        std::cout << "ImageData saved successfully to " << imgPath << std::endl;
    } else {
        std::cerr << "Failed to save ImageData!" << std::endl;
    }

    return 0;
}
*/
