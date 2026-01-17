#include "src/engine/Engine.hpp"
#include "src/data/TextData.hpp"
#include "src/module/TextLengthModule.hpp"
#include <iostream>
#include <memory>

int main() {
    std::cout << "DMPS Backend Test Starting...\n";

    // Créer une donnée texte
    auto textData = std::make_shared<TextData>("Hello DMPS Architecture!", TextFormat::TXT);

    // Créer le moteur
    Engine engine;
    engine.setData(textData);

    // Ajouter un module texte
    auto textLengthModule = std::make_shared<TextLengthModule>();
    engine.addModule(textLengthModule);

    // Appliquer tous les modules
    ModuleResult result = engine.applyModules();

    // Afficher les messages / problèmes
    if (!result.issues.empty()) {
        std::cout << "=== Issues ===\n";
        for (auto& issue : result.issues) {
            std::cout << "  [" << static_cast<int>(issue.type) << "] "
                      << issue.code << ": " << issue.message << "\n";
        }
    }

    if (result.success) {
        std::cout << "Result summary:\n" << result.message << "\n";
    }

    // Récupérer la donnée modifiée
    auto finalData = engine.getData();
    if (finalData && finalData->type() == DataType::Text) {
        auto textPtr = std::dynamic_pointer_cast<TextData>(finalData);
        std::cout << "Final Data content:\n" << textPtr->getContent() << "\n";
    }

    std::cout << "DMPS Backend Test Finished.\n";
    return 0;
}
