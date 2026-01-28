#ifndef SAVEFILEDIALOGHELPER_HPP
#define SAVEFILEDIALOGHELPER_HPP

#include <QString>
#include <QStringList>
#include "../../data/IData.hpp"
#include "../../DataSaveManager/DataSaveManager.hpp"

/**
 * @brief Helper class to generate QFileDialog filters based on data type
 *
 * Converts supported extensions from DataSaveManager into Qt filter strings.
 */
class SaveFileDialogHelper {
public:
    /**
     * @brief Generate file filter string for QFileDialog
     *
     * @param data The data to be saved
     * @param saveManager The save manager with registered savers
     * @return QString filter like "Text Files (*.txt);;JSON Files (*.json);;All Files (*)"
     */
    static QString generateFilter(const IData& data, const DataSaveManager& saveManager) {
        QStringList filters;

        // Get available formats
        auto formats = saveManager.availableFormats(data);

        // Group by category based on data type
        if (data.type() == DataType::Text) {
            QStringList textFormats;
            for (const auto& ext : formats) {
                if (ext == "txt") textFormats << "*.txt";
                if (ext == "json") textFormats << "*.json";
            }
            if (!textFormats.isEmpty()) {
                filters << QString("Text Files (%1)").arg(textFormats.join(" "));
            }
        }
        else if (data.type() == DataType::Image) {
            QStringList imageFormats;
            for (const auto& ext : formats) {
                if (ext == "png") imageFormats << "*.png";
                if (ext == "jpg") imageFormats << "*.jpg";
                if (ext == "bmp") imageFormats << "*.bmp";
            }
            if (!imageFormats.isEmpty()) {
                filters << QString("Image Files (%1)").arg(imageFormats.join(" "));
            }
        }
        else if (data.type() == DataType::Tabular) {
            QStringList tabularFormats;
            for (const auto& ext : formats) {
                if (ext == "csv") tabularFormats << "*.csv";
                if (ext == "xlsx") tabularFormats << "*.xlsx";
            }
            if (!tabularFormats.isEmpty()) {
                filters << QString("Tabular Files (%1)").arg(tabularFormats.join(" "));
            }
        }

        // Add individual format filters
        for (const auto& ext : formats) {
            QString upperExt = QString::fromStdString(ext).toUpper();
            filters << QString("%1 Files (*.%2)").arg(upperExt).arg(QString::fromStdString(ext));
        }

        // Add "All Files" at the end
        filters << "All Files (*)";

        return filters.join(";;");
    }

    /**
     * @brief Get default extension based on data type
     *
     * @param data The data to be saved
     * @return QString default extension (e.g., "txt", "png", "csv")
     */
    static QString getDefaultExtension(const IData& data) {
        switch (data.type()) {
        case DataType::Text:
            return "txt";
        case DataType::Image:
            return "png";
        case DataType::Tabular:
            return "csv";
        default:
            return "txt";
        }
    }
};

#endif // SAVEFILEDIALOGHELPER_HPP
