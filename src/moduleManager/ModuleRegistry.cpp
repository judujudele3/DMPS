#include "ModuleRegistry.hpp"
#include <algorithm>
#include <set>

// Include all your module headers here
// Example:
 #include "../module/TextLengthModule.hpp"
// #include "TabularFillMissingModule.h"
// #include "BlurModule.h"
// etc.

const std::vector<ModuleInfo>& ModuleRegistry::getAvailableModules() {
    static std::vector<ModuleInfo> registry = initializeRegistry();
    return registry;
}

const ModuleInfo* ModuleRegistry::findModule(const std::string& name) {
    const auto& modules = getAvailableModules();
    auto it = std::find_if(modules.begin(), modules.end(),
                           [&name](const ModuleInfo& info) {
                               return info.name == name;
                           });
    return (it != modules.end()) ? &(*it) : nullptr;
}

std::vector<const ModuleInfo*> ModuleRegistry::getModulesByCategory(const std::string& category) {
    std::vector<const ModuleInfo*> result;
    const auto& modules = getAvailableModules();

    for (const auto& module : modules) {
        if (module.category == category) {
            result.push_back(&module);
        }
    }

    return result;
}

std::vector<std::string> ModuleRegistry::getCategories() {
    std::set<std::string> categories;
    const auto& modules = getAvailableModules();

    for (const auto& module : modules) {
        categories.insert(module.category);
    }

    return std::vector<std::string>(categories.begin(), categories.end());
}

std::vector<ModuleInfo> ModuleRegistry::initializeRegistry() {
    std::vector<ModuleInfo> modules;

    // ============================================================
    // REGISTRATION SECTION
    // Your teammate will have added getParameterDescriptors() to all modules
    // Now you register them here following the template below
    // ============================================================

    // ------------------------------------------------------------
    // TEMPLATE FOR MODULE WITHOUT PARAMETERS:
    // ------------------------------------------------------------
    // modules.push_back({
    //     "TextLengthModule",                    // name (unique identifier)
    //     "Text Length",                         // displayName
    //     "Text Processing",                     // category
    //     {},                                    // no parameters
    //     [](const ParamMap&) {                 // factory
    //         return std::make_shared<TextLengthModule>();
    //     }
    // });

    // ------------------------------------------------------------
    // TEMPLATE FOR MODULE WITH PARAMETERS:
    // ------------------------------------------------------------
    // modules.push_back({
    //     "TabularFillMissingModule",            // name
    //     "Fill Missing Values",                 // displayName
    //     "Data Cleaning",                       // category
    //     TabularFillMissingModule::getParameterDescriptors(), // parameters from module
    //     [](const ParamMap& params) {          // factory
    //         std::string column = std::get<std::string>(params.at("column"));
    //         std::string fillValue = std::get<std::string>(params.at("fillValue"));
    //         return std::make_shared<TabularFillMissingModule>(column, fillValue);
    //     }
    // });

    // ------------------------------------------------------------
    // EXAMPLE: Module with int parameter
    // ------------------------------------------------------------
    // modules.push_back({
    //     "BlurModule",
    //     "Blur Filter",
    //     "Image Processing",
    //     BlurModule::getParameterDescriptors(),
    //     [](const ParamMap& params) {
    //         int radius = std::get<int>(params.at("radius"));
    //         return std::make_shared<BlurModule>(radius);
    //     }
    // });

    // ------------------------------------------------------------
    // EXAMPLE: Module with multiple parameters of different types
    // ------------------------------------------------------------
    // modules.push_back({
    //     "ResizeImageModule",
    //     "Resize Image",
    //     "Image Processing",
    //     ResizeImageModule::getParameterDescriptors(),
    //     [](const ParamMap& params) {
    //         int width = std::get<int>(params.at("width"));
    //         int height = std::get<int>(params.at("height"));
    //         bool keepAspect = std::get<bool>(params.at("keepAspectRatio"));
    //         return std::make_shared<ResizeImageModule>(width, height, keepAspect);
    //     }
    // });

    // ============================================================
    // ADD YOUR 35 MODULES HERE
    // Follow the templates above
    // ============================================================

    // Example registrations (commented out - replace with your actual modules):

    /*
    // Text Processing Category
    modules.push_back({
        "TextLengthModule",
        "Calculate Text Length",
        "Text Processing",
        {},
        [](const ParamMap&) {
            return std::make_shared<TextLengthModule>();
        }
    });

    modules.push_back({
        "WordCountModule",
        "Count Words",
        "Text Processing",
        {},
        [](const ParamMap&) {
            return std::make_shared<WordCountModule>();
        }
    });

    // Data Cleaning Category
    modules.push_back({
        "TabularFillMissingModule",
        "Fill Missing Values",
        "Data Cleaning",
        TabularFillMissingModule::getParameterDescriptors(),
        [](const ParamMap& params) {
            std::string column = std::get<std::string>(params.at("column"));
            std::string fillValue = std::get<std::string>(params.at("fillValue"));
            return std::make_shared<TabularFillMissingModule>(column, fillValue);
        }
    });

    // Image Processing Category
    modules.push_back({
        "GrayscaleModule",
        "Convert to Grayscale",
        "Image Processing",
        {},
        [](const ParamMap&) {
            return std::make_shared<GrayscaleModule>();
        }
    });

    modules.push_back({
        "BlurModule",
        "Blur Image",
        "Image Processing",
        BlurModule::getParameterDescriptors(),
        [](const ParamMap& params) {
            int radius = std::get<int>(params.at("radius"));
            return std::make_shared<BlurModule>(radius);
        }
    });
    */
    modules.push_back({
        "TextLengthModule",           // Nom unique (doit matcher le nom de la classe)
        "Calculate Text Length",      // Nom affiché dans l'UI
        "Text Processing",            // Catégorie pour organiser l'UI
        {},                          // Pas de paramètres
        [](const ParamMap&) {        // Factory lambda
            return std::make_shared<TextLengthModule>();
        }
    });

    return modules;
}
