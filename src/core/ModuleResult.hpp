#ifndef MODULERESULT_HPP
#define MODULERESULT_HPP

#include <string>
#include <vector>
#include "ModuleError.hpp"

struct ModuleResult {
    bool success = true;                  // true si traitement OK
    std::string message;                       // message global ou résumé
    std::vector<ModuleError> issues;           // erreurs & warnings

    // méthode utilitaire pour ajouter une erreur
    void addError(ErrorType type, const std::string& code, const std::string& msg) {
        issues.push_back({type, code, msg});
        if (type == ErrorType::Error || type == ErrorType::Critical) {
            success = false;
        }
    }
};

#endif
