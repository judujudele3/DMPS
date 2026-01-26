/*#include "TabularDescribe.hpp"
#include "TabularSumModule.hpp"
#include "TabularCountModule.hpp"
#include "TabularMeanModule.hpp"
#include "TabularStdModule.hpp"
#include "TabularMinModule.hpp"
#include "TabularMaxModule.hpp"
#include "TabularMedianModule.hpp"
#include "TabularIsNullModule.hpp"

std::string TabularDescribe::getName() const {
    return "Tabular Summary Module";
}

ModuleResult TabularDescribe::apply(IData& data) {
    ModuleResult res;
    if (data.type() != DataType::Tabular) {
        res.addError(ErrorType::Error, "INVALID_DATA_TYPE", "TabularSummaryModule expects TabularData.");
        return res;
    }

    TabularData& table = dynamic_cast<TabularData&>(data);

    // Boucle sur toutes les colonnes numériques
    for (const auto& col : table.header()) {
        int idx = table.columnIndex(col);
        bool isNumeric = true;
        for (const auto& row : table.rows()) {
            if (!row[idx].empty()) {
                try { std::stod(row[idx]); } catch(...) { isNumeric = false; break; }
            }
        }
        if (!isNumeric) continue;

        // Calculer les stats
        TabularSumModule sumMod(col);
        TabularCountModule countMod(col);
        TabularMeanModule meanMod(col);
        TabularStdModule stdMod(col);
        TabularMinModule minMod(col);
        TabularMaxModule maxMod(col);
        TabularMedianModule medianMod(col);
        TabularIsNullModule isNullMod(col);

        res.set(col + "_sum", sumMod.apply(table).get<double>("sum"));
        res.set(col + "_count", countMod.apply(table).get<int>("count"));
        res.set(col + "_mean", meanMod.apply(table).get<double>("mean"));
        res.set(col + "_std", stdMod.apply(table).get<double>("std"));
        res.set(col + "_min", minMod.apply(table).get<double>("min"));
        res.set(col + "_max", maxMod.apply(table).get<double>("max"));
        res.set(col + "_median", medianMod.apply(table).get<double>("median"));
        res.set(col + "_isnull", isNullMod.apply(table).get<int>("isnull"));

    }

    res.message = "Summary computed for numeric columns.";
    return res;
}


//// methode avec le threads+mutex*/






// ----------------- at vs buffer vs parallel

/*



#include <iostream>
#include <cstdint>
#include <chrono>
#include <random>
#include <vector>
#include <omp.h>


// ------------------ Utils ------------------

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

// ------------------ Méthodes ------------------

void grayscaleAt(ImageData& img) {
    for (int y = 0; y < img.getHeight(); ++y) {
        for (int x = 0; x < img.getWidth(); ++x) {
            Pixel& p = img.at(x, y);
            uint8_t gray = (p.r + p.g + p.b) / 3;
            p.r = p.g = p.b = gray;
        }
    }
}

void grayscaleBuffer(ImageData& img) {
    auto& pixels = img.pixels();
    for (Pixel& p : pixels) {
        uint8_t gray = (p.r + p.g + p.b) / 3;
        p.r = p.g = p.b = gray;
    }
}

void grayscaleAtParallel(ImageData& img) {
    int width = img.getWidth();
    int height = img.getHeight();

#pragma omp parallel for
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            Pixel& p = img.at(x, y);
            uint8_t gray = (p.r + p.g + p.b) / 3;
            p.r = p.g = p.b = gray;
        }
    }
}

// ------------------ Benchmark ------------------

double measure(void (*func)(ImageData&), ImageData& img) {
    auto start = std::chrono::high_resolution_clock::now();
    func(img);
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double>(end - start).count();
}

int main() {

    std::vector<std::pair<int, int>> sizes = {
        {256,256}, {384,384}, {512,512}, {640,640}, {768,768},
        {896,896}, {1024,1024}, {1280,720}, {1600,900},
        {1920,1080}, {1920,1500}, {2560,1440},
        {3000,2000}, {3840,2160}, {4096,4096}
    };

    std::cout << "Width x Height | at(x,y) | buffer | parallel\n";
    std::cout << "----------------------------------------------\n";

    for (auto& [w, h] : sizes) {
        ImageData img(w, h, ImageFormat::JPG);
        fillRandom(img);

        ImageData img1 = img;
        ImageData img2 = img;
        ImageData img3 = img;

        double t1 = measure(grayscaleAt, img1);
        double t2 = measure(grayscaleBuffer, img2);
        double t3 = measure(grayscaleAtParallel, img3);

        std::cout << w << "x" << h << " | "
                  << t1 << " | "
                  << t2 << " | "
                  << t3 << "\n";
    }

    return 0;
}



#pragma omp parallel for
⭐ Ligne la plus importante
C’est une directive OpenMP (un “indice” donné au compilateur).

Elle signifie :

“Cette boucle for peut être divisée et exécutée par plusieurs threads en parallèle.”

for (int y = 0; y < height; ++y) {
Boucle parallélisée

Chaque valeur de y peut être traitée indépendamment

OpenMP va :

créer plusieurs threads

répartir les valeurs de y entre eux

Exemple (4 cœurs) :

Thread 0 → y = 0 .. 374
Thread 1 → y = 375 .. 749
Thread 2 → y = 750 .. 1124
Thread 3 → y = 1125 .. 1499


for (int x = 0; x < width; ++x) {
Boucle interne

Exécutée séquentiellement à l’intérieur de chaque thread

Très important pour :

le cache mémoire

éviter les conflits

Pixel& p = img.at(x, y);
Chaque thread accède à un pixel différent

Aucun partage de pixel entre threads

👉 Pas de data race ✅




*/



