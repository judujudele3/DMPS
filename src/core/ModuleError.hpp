#ifndef MODULEERROR_HPP
#define MODULEERROR_HPP

#include <string>

enum class ErrorType {
    Info,
    Warning,
    Error,
    Critical
};

struct ModuleError {
    ErrorType type;
    std::string code;     // identifiant unique de l'erreur
    std::string message;  // message lisible par l'utilisateur
};

#endif // MODULEERROR_HPP
