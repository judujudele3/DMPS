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
}*/
#include <iostream>
#include <chrono>
#include <iostream>
#include <vector>
#include<random>
#include "src/module/ImageBlurModule.hpp"
#include "src/module/ImageFlipModule.hpp"
#include "src/module/ImageInvertColorsModule.hpp"
#include "src/module/ImageResizeModule.hpp"
#include "src/module/ImageColorHistogramModule.hpp"

void fillRandom(ImageData& img) {
    std::mt19937 gen(42);
    std::uniform_int_distribution<int> dist(0, 255);

    for (int y = 0; y < img.getHeight(); ++y) {
        for (int x = 0; x < img.getWidth(); ++x) {
            Pixel& p = img.at(x, y);
            p.r = dist(gen);
            p.g = dist(gen);
            p.b = dist(gen);
            p.a = 255;
        }
    }
}

int main() {
    const int width = 256;
    const int height = 256;

    ImageData img(width, height, ImageFormat::JPG);
    fillRandom(img);


    // 🔹 Color Histogram
    ImageColorHistogramModule histModule;
    ModuleResult histRes = histModule.apply(img);
    std::cout << histRes.message << "\n";
    // Affiche un échantillon de l'histogramme rouge

    auto histR = histRes.get<std::vector<size_t>>("HistR");
    std::cout << "HistR sample: ";
    for (int i = 0; i < 10; ++i) std::cout << histR[i] << " ";
    std::cout << "...\n";

    // 🔹 Blur
    ImageBlurModule blurModule;
    ModuleResult blurRes = blurModule.apply(img);
    std::cout << blurRes.message << "\n";

    // 🔹 Flip Horizontal
    ImageFlipModule flipModule(FlipType::Horizontal);
    ModuleResult flipRes = flipModule.apply(img);
    std::cout << flipRes.message << "\n";

    // 🔹 Invert Colors
    ImageInvertColorsModule invertModule;
    ModuleResult invertRes = invertModule.apply(img);
    std::cout << invertRes.message << "\n";

    // 🔹 Resize
    ImageResizeModule resizeModule(128, 128); // réduire
    ModuleResult resizeRes = resizeModule.apply(img);
    std::cout << resizeRes.message << "\n";
    std::cout << "New size: " << img.getWidth() << "x" << img.getHeight() << "\n";



    return 0;
}
