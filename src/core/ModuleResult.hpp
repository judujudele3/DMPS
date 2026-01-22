#ifndef MODULERESULT_HPP
#define MODULERESULT_HPP

#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <variant>
#include "ModuleError.hpp"

// Définition du type générique pour les valeurs
using ResultValue = std::variant<
    int,
    double,
    std::string,
    bool,
    std::vector<double>,
    std::vector<std::string>
    >;

struct ModuleResult {
    bool success = true;
    std::string message;

    // clé du design
    std::unordered_map<std::string, ResultValue> values;

    std::vector<ModuleError> issues;

    void addError(ErrorType type,
                  const std::string& code,
                  const std::string& msg) {
        issues.push_back({type, code, msg});
        if (type == ErrorType::Error || type == ErrorType::Critical)
            success = false;
    }

    // Helpers
    bool has(const std::string& key) const {
        return values.count(key) > 0;
    }

    template<typename T>
    T get(const std::string& key) const {
        return std::get<T>(values.at(key));
    }

    template<typename T>
    void set(const std::string& key, T value) {
        values[key] = value;
    }
};

#endif



