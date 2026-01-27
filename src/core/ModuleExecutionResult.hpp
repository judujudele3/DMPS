#ifndef MODULEEXECUTIONRESULT_HPP
#define MODULEEXECUTIONRESULT_HPP

#include "ModuleResult.hpp"
#include <string>
#include <chrono>

/**
 * @brief Result of executing a single module
 *
 * Contains:
 * - Module identification
 * - Execution result (values, errors, warnings)
 * - Performance metrics
 */
struct ModuleExecutionResult {
    std::string moduleName;              // Name of the module that was executed
    std::string displayName;             // Human-readable name (from ModuleInfo)
    ModuleResult result;                 // The actual result with values and errors
    std::chrono::milliseconds duration;  // Execution time

    /**
     * @brief Check if the module execution was successful
     */
    bool isSuccess() const {
        return result.success;
    }

    /**
     * @brief Check if there are any errors
     */
    bool hasErrors() const {
        for (const auto& issue : result.issues) {
            if (issue.type == ErrorType::Error || issue.type == ErrorType::Critical) {
                return true;
            }
        }
        return false;
    }

    /**
     * @brief Check if there are any warnings
     */
    bool hasWarnings() const {
        for (const auto& issue : result.issues) {
            if (issue.type == ErrorType::Warning) {
                return true;
            }
        }
        return false;
    }

    /**
     * @brief Get icon name based on result status
     */
    std::string getStatusIcon() const {
        if (!isSuccess() || hasErrors()) {
            return "error";
        }
        if (hasWarnings()) {
            return "warning";
        }
        return "success";
    }
};

#endif // MODULEEXECUTIONRESULT_HPP
