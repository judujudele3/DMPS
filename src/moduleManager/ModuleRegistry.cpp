#include "ModuleRegistry.hpp"
#include <algorithm>
#include <set>
// ============================================================
// MODULE INCLUDES - 35 MODULES
// ============================================================

// ------------------------------------------------------------
// IMAGE PROCESSING MODULES (10 modules)
// ------------------------------------------------------------

#include "../module/ImageAverageColorModule.hpp"
#include "../module/ImageBlurModule.hpp"
#include "../module/ImageBrightnessModule.hpp"
#include "../module/ImageColorHistogramModule.hpp"
#include "../module/ImageFlipModule.hpp"
#include "../module/ImageGrayScaleModule.hpp"
#include "../module/ImageInvertColorsModule.hpp"
#include "../module/ImageResizeModule.hpp"
#include "../module/ImageRotateModule.hpp"
#include "../module/ImageSizeModule.hpp"

// ------------------------------------------------------------
// DATA CLEANING / TABULAR MODULES (13 modules)
// ------------------------------------------------------------

#include "../module/TabularCountModule.hpp"
#include "../module/TabularDescribe.hpp"
#include "../module/TabularDropEmptyRowsModule.hpp"
#include "../module/TabularFillMissingValuesModule.hpp"
#include "../module/TabularFilterNumericColumnsModule.hpp"
#include "../module/TabularIsNullModule.hpp"
#include "../module/TabularMaxModule.hpp"
#include "../module/TabularMeanModule.hpp"
#include "../module/TabularMedianModule.hpp"
#include "../module/TabularMinModule.hpp"
#include "../module/TabularNormalizeModule.hpp"
#include "../module/TabularStdModule.hpp"
#include "../module/TabularSumModule.hpp"

// ------------------------------------------------------------
// TEXT PROCESSING MODULES (12 modules)
// ------------------------------------------------------------

#include "../module/TextCharacterCountModule.hpp"
#include "../module/TextLanguageDetectionModule.hpp"
#include "../module/TextLengthDistributionModule.hpp"
#include "../module/TextLengthModule.hpp"
#include "../module/TextLineCountModule.hpp"
#include "../module/TextLowerFirstLetterModule.hpp"
#include "../module/TextRemoveExtraSpacesModule.hpp"
#include "../module/TextTokenizeModule.hpp"
#include "../module/TextTopWordsModule.hpp"
#include "../module/TextTrimLinesModule.hpp"
#include "../module/TextWordCountModule.hpp"

// ============================================================
// END OF MODULE INCLUDES
// Total: 35 modules
// ============================================================

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
    // REGISTRATION SECTION - 35 MODULES
    // ============================================================

    // ------------------------------------------------------------
    // IMAGE PROCESSING CATEGORY (10 modules)
    // ------------------------------------------------------------

    modules.push_back({
        "ImageAverageColorModule",
        "Average Color",
        "Image Processing",
        {},
        [](const ParamMap&) {
            return std::make_shared<ImageAverageColorModule>();
        }
    });

    modules.push_back({
        "ImageBlurModule",
        "Blur Image",
        "Image Processing",
        {},
        [](const ParamMap&) {
            return std::make_shared<ImageBlurModule>();
        }
    });

    modules.push_back({
        "ImageBrightnessModule",
        "Adjust Brightness",
        "Image Processing",
        {},
        [](const ParamMap&) {
            return std::make_shared<ImageBrightnessModule>();
        }
    });

    modules.push_back({
        "ImageColorHistogramModule",
        "Color Histogram",
        "Image Processing",
        {},
        [](const ParamMap&) {
            return std::make_shared<ImageColorHistogramModule>();
        }
    });

    modules.push_back({
        "ImageFlipModule",
        "Flip Image",
        "Image Processing",
        ImageFlipModule::getParameterDescriptors(),
        [](const ParamMap& params) {
            std::string typeStr = std::get<std::string>(params.at("type"));
            FlipType type = (typeStr == "Horizontal") ? FlipType::Horizontal : FlipType::Vertical;
            return std::make_shared<ImageFlipModule>(type);
        }
    });

    modules.push_back({
        "ImageGrayScaleModule",
        "Convert to Grayscale",
        "Image Processing",
        {},
        [](const ParamMap&) {
            return std::make_shared<ImageGrayScaleModule>();
        }
    });

    modules.push_back({
        "ImageInvertColorsModule",
        "Invert Colors",
        "Image Processing",
        {},
        [](const ParamMap&) {
            return std::make_shared<ImageInvertColorsModule>();
        }
    });

    modules.push_back({
        "ImageResizeModule",
        "Resize Image",
        "Image Processing",
        ImageResizeModule::getParameterDescriptors(),
        [](const ParamMap& params) {
            int width = std::get<int>(params.at("width"));
            int height = std::get<int>(params.at("height"));
            return std::make_shared<ImageResizeModule>(width, height);
        }
    });

    modules.push_back({
        "ImageRotateModule",
        "Rotate Image",
        "Image Processing",
        ImageRotateModule::getParameterDescriptors(),
        [](const ParamMap& params) {
            std::string degreeStr = std::get<std::string>(params.at("degree"));
            RotateDegree degree;
            if (degreeStr == "R90") degree = RotateDegree::R90;
            else if (degreeStr == "R180") degree = RotateDegree::R180;
            else degree = RotateDegree::R270;
            return std::make_shared<ImageRotateModule>(degree);
        }
    });

    modules.push_back({
        "ImageSizeModule",
        "Get Image Size",
        "Image Processing",
        {},
        [](const ParamMap&) {
            return std::make_shared<ImageSizeModule>();
        }
    });

    // ------------------------------------------------------------
    // DATA CLEANING / TABULAR CATEGORY (13 modules)
    // ------------------------------------------------------------

    modules.push_back({
        "TabularCountModule",
        "Count Values",
        "Data Cleaning",
        TabularCountModule::getParameterDescriptors(),
        [](const ParamMap& params) {
            std::string colName = std::get<std::string>(params.at("colName"));
            return std::make_shared<TabularCountModule>(colName);
        }
    });

    modules.push_back({
        "TabularDescribe",
        "Describe Data",
        "Data Cleaning",
        {},
        [](const ParamMap&) {
            return std::make_shared<TabularDescribe>();
        }
    });

    modules.push_back({
        "TabularDropEmptyRowsModule",
        "Drop Empty Rows",
        "Data Cleaning",
        {},
        [](const ParamMap&) {
            return std::make_shared<TabularDropEmptyRowsModule>();
        }
    });

    modules.push_back({
        "TabularFillMissingValuesModule",
        "Fill Missing Values",
        "Data Cleaning",
        TabularFillMissingValuesModule::getParameterDescriptors(),
        [](const ParamMap& params) {
            std::string column = std::get<std::string>(params.at("column"));
            std::string fillValue = std::get<std::string>(params.at("fillValue"));
            return std::make_shared<TabularFillMissingValuesModule>(column, fillValue);
        }
    });

    modules.push_back({
        "TabularFilterNumericColumnsModule",
        "Filter Numeric Columns",
        "Data Cleaning",
        {},
        [](const ParamMap&) {
            return std::make_shared<TabularFilterNumericColumnsModule>();
        }
    });

    modules.push_back({
        "TabularIsNullModule",
        "Check Null Values",
        "Data Cleaning",
        TabularIsNullModule::getParameterDescriptors(),
        [](const ParamMap& params) {
            std::string colName = std::get<std::string>(params.at("colName"));
            return std::make_shared<TabularIsNullModule>(colName);
        }
    });

    modules.push_back({
        "TabularMaxModule",
        "Calculate Maximum",
        "Data Cleaning",
        TabularMaxModule::getParameterDescriptors(),
        [](const ParamMap& params) {
            std::string colName = std::get<std::string>(params.at("colName"));
            return std::make_shared<TabularMaxModule>(colName);
        }
    });

    modules.push_back({
        "TabularMeanModule",
        "Calculate Mean",
        "Data Cleaning",
        TabularMeanModule::getParameterDescriptors(),
        [](const ParamMap& params) {
            std::string colName = std::get<std::string>(params.at("colName"));
            return std::make_shared<TabularMeanModule>(colName);
        }
    });

    modules.push_back({
        "TabularMedianModule",
        "Calculate Median",
        "Data Cleaning",
        TabularMedianModule::getParameterDescriptors(),
        [](const ParamMap& params) {
            std::string colName = std::get<std::string>(params.at("colName"));
            return std::make_shared<TabularMedianModule>(colName);
        }
    });

    modules.push_back({
        "TabularMinModule",
        "Calculate Minimum",
        "Data Cleaning",
        TabularMinModule::getParameterDescriptors(),
        [](const ParamMap& params) {
            std::string colName = std::get<std::string>(params.at("colName"));
            return std::make_shared<TabularMinModule>(colName);
        }
    });

    modules.push_back({
        "TabularNormalizeModule",
        "Normalize Data",
        "Data Cleaning",
        {},
        [](const ParamMap&) {
            return std::make_shared<TabularNormalizeModule>();
        }
    });

    modules.push_back({
        "TabularStdModule",
        "Calculate Standard Deviation",
        "Data Cleaning",
        TabularStdModule::getParameterDescriptors(),
        [](const ParamMap& params) {
            std::string colName = std::get<std::string>(params.at("colName"));
            return std::make_shared<TabularStdModule>(colName);
        }
    });

    modules.push_back({
        "TabularSumModule",
        "Calculate Sum",
        "Data Cleaning",
        TabularSumModule::getParameterDescriptors(),
        [](const ParamMap& params) {
            std::string columnName = std::get<std::string>(params.at("columnName"));
            return std::make_shared<TabularSumModule>(columnName);
        }
    });

    // ------------------------------------------------------------
    // TEXT PROCESSING CATEGORY (12 modules)
    // ------------------------------------------------------------

    modules.push_back({
        "TextCharacterCountModule",
        "Count Characters",
        "Text Processing",
        {},
        [](const ParamMap&) {
            return std::make_shared<TextCharacterCountModule>();
        }
    });

    modules.push_back({
        "TextLanguageDetectionModule",
        "Detect Language",
        "Text Processing",
        {},
        [](const ParamMap&) {
            return std::make_shared<TextLanguageDetectionModule>();
        }
    });

    modules.push_back({
        "TextLengthDistributionModule",
        "Length Distribution",
        "Text Processing",
        {},
        [](const ParamMap&) {
            return std::make_shared<TextLengthDistributionModule>();
        }
    });

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
        "TextLineCountModule",
        "Count Lines",
        "Text Processing",
        {},
        [](const ParamMap&) {
            return std::make_shared<TextLineCountModule>();
        }
    });

    modules.push_back({
        "TextLowerFirstLetterModule",
        "Lowercase First Letter",
        "Text Processing",
        {},
        [](const ParamMap&) {
            return std::make_shared<TextLowerFirstLetterModule>();
        }
    });

    modules.push_back({
        "TextRemoveExtraSpacesModule",
        "Remove Extra Spaces",
        "Text Processing",
        {},
        [](const ParamMap&) {
            return std::make_shared<TextRemoveExtraSpacesModule>();
        }
    });

    modules.push_back({
        "TextTokenizeModule",
        "Tokenize Text",
        "Text Processing",
        {},
        [](const ParamMap&) {
            return std::make_shared<TextTokenizeModule>();
        }
    });

    modules.push_back({
        "TextTopWordsModule",
        "Top Frequent Words",
        "Text Processing",
        TextTopWordsModule::getParameterDescriptors(),
        [](const ParamMap& params) {
            int topK = std::get<int>(params.at("topK"));
            return std::make_shared<TextTopWordsModule>(topK);
        }
    });

    modules.push_back({
        "TextTrimLinesModule",
        "Trim Lines",
        "Text Processing",
        {},
        [](const ParamMap&) {
            return std::make_shared<TextTrimLinesModule>();
        }
    });

    modules.push_back({
        "TextWordCountModule",
        "Count Words",
        "Text Processing",
        {},
        [](const ParamMap&) {
            return std::make_shared<TextWordCountModule>();
        }
    });

    // ============================================================
    // END OF MODULE REGISTRATION
    // Total: 35 modules
    // - Image Processing: 10 modules
    // - Data Cleaning: 13 modules
    // - Text Processing: 12 modules
    // ============================================================

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
