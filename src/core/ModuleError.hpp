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
    std::string code;     // Unique error identifier
    std::string message;  // Unique error ID
};

#endif // MODULEERROR_HPP
