#ifndef BASICRESULTRENDERERS_HPP
#define BASICRESULTRENDERERS_HPP

#include "IResultValueRenderer.hpp"
#include <sstream>
#include <iomanip>

/**
 * @brief Renderer for int values
 */
class IntRenderer : public IResultValueRenderer {
public:
    bool canRender(const ResultValue& value) const override {
        return std::holds_alternative<int>(value);
    }

    QString render(const std::string& key, const ResultValue& value) const override {
        int val = std::get<int>(value);
        return QString::fromStdString(key + ": ") + QString::number(val);
    }
};

/**
 * @brief Renderer for size_t values
 */
class SizeTRenderer : public IResultValueRenderer {
public:
    bool canRender(const ResultValue& value) const override {
        return std::holds_alternative<size_t>(value);
    }

    QString render(const std::string& key, const ResultValue& value) const override {
        size_t val = std::get<size_t>(value);
        return QString::fromStdString(key + ": ") + QString::number(static_cast<qulonglong>(val));
    }
};

/**
 * @brief Renderer for double values
 */
class DoubleRenderer : public IResultValueRenderer {
public:
    bool canRender(const ResultValue& value) const override {
        return std::holds_alternative<double>(value);
    }

    QString render(const std::string& key, const ResultValue& value) const override {
        double val = std::get<double>(value);
        return QString::fromStdString(key + ": ") + QString::number(val, 'f', 3);
    }
};

/**
 * @brief Renderer for string values
 */
class StringRenderer : public IResultValueRenderer {
public:
    bool canRender(const ResultValue& value) const override {
        return std::holds_alternative<std::string>(value);
    }

    QString render(const std::string& key, const ResultValue& value) const override {
        std::string val = std::get<std::string>(value);

        // Truncate long strings for tree view
        QString qval = QString::fromStdString(val);
        if (qval.length() > 50) {
            qval = qval.left(47) + "...";
        }

        return QString::fromStdString(key + ": ") + qval;
    }
};

/**
 * @brief Renderer for bool values
 */
class BoolRenderer : public IResultValueRenderer {
public:
    bool canRender(const ResultValue& value) const override {
        return std::holds_alternative<bool>(value);
    }

    QString render(const std::string& key, const ResultValue& value) const override {
        bool val = std::get<bool>(value);
        return QString::fromStdString(key + ": ") + (val ? "true" : "false");
    }
};

/**
 * @brief Renderer for vector<double> values
 */
class VectorDoubleRenderer : public IResultValueRenderer {
public:
    bool canRender(const ResultValue& value) const override {
        return std::holds_alternative<std::vector<double>>(value);
    }

    QString render(const std::string& key, const ResultValue& value) const override {
        const auto& vec = std::get<std::vector<double>>(value);

        if (vec.empty()) {
            return QString::fromStdString(key + ": []");
        }

        // Show first few elements
        QString result = QString::fromStdString(key + ": [");
        size_t displayCount = std::min(vec.size(), size_t(3));

        for (size_t i = 0; i < displayCount; ++i) {
            if (i > 0) result += ", ";
            result += QString::number(vec[i], 'f', 2);
        }

        if (vec.size() > displayCount) {
            result += ", ... (" + QString::number(vec.size()) + " items)";
        }

        result += "]";
        return result;
    }
};

/**
 * @brief Renderer for vector<string> values
 */
class VectorStringRenderer : public IResultValueRenderer {
public:
    bool canRender(const ResultValue& value) const override {
        return std::holds_alternative<std::vector<std::string>>(value);
    }

    QString render(const std::string& key, const ResultValue& value) const override {
        const auto& vec = std::get<std::vector<std::string>>(value);

        if (vec.empty()) {
            return QString::fromStdString(key + ": []");
        }

        // Show first few elements
        QString result = QString::fromStdString(key + ": [");
        size_t displayCount = std::min(vec.size(), size_t(3));

        for (size_t i = 0; i < displayCount; ++i) {
            if (i > 0) result += ", ";
            QString item = QString::fromStdString(vec[i]);
            if (item.length() > 20) {
                item = item.left(17) + "...";
            }
            result += "\"" + item + "\"";
        }

        if (vec.size() > displayCount) {
            result += ", ... (" + QString::number(vec.size()) + " items)";
        }

        result += "]";
        return result;
    }
};

/**
 * @brief Fallback renderer for unknown types
 */
class FallbackRenderer : public IResultValueRenderer {
public:
    bool canRender(const ResultValue& value) const override {
        return true; // Can handle anything as last resort
    }

    QString render(const std::string& key, const ResultValue& value) const override {
        return QString::fromStdString(key + ": <unsupported type>");
    }

    int getPriority() const override {
        return -1000; // Lowest priority, used only as fallback
    }
};

#endif // BASICRESULTRENDERERS_HPP
