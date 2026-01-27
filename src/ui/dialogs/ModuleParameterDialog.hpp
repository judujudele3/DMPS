#ifndef MODULEPARAMETERDIALOG_HPP
#define MODULEPARAMETERDIALOG_HPP

#include <QDialog>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QLabel>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QCheckBox>
#include <map>
#include "../../core/ModuleParameter.hpp"

/**
 * @brief Dynamic dialog for configuring module parameters
 *
 * This dialog is generated dynamically based on the module's parameter descriptors.
 * It creates appropriate input widgets for each parameter type:
 * - QSpinBox for int parameters
 * - QDoubleSpinBox for float parameters
 * - QLineEdit for string parameters
 * - QCheckBox for bool parameters
 *
 * The dialog enforces constraints (min/max) automatically.
 */
class ModuleParameterDialog : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Construct a parameter configuration dialog
     * @param moduleName Display name of the module being configured
     * @param parameters Parameter descriptors from the module
     * @param currentValues Current parameter values (or defaults)
     * @param parent Parent widget
     */
    explicit ModuleParameterDialog(const std::string& moduleName,
                                   const std::vector<ModuleParameter>& parameters,
                                   const ParamMap& currentValues,
                                   QWidget *parent = nullptr);
    ~ModuleParameterDialog();

    /**
     * @brief Get the configured parameter values
     * @return Map of parameter name to value
     */
    ParamMap getParameters() const;

private:
    /**
     * @brief Build the UI dynamically based on parameter descriptors
     */
    void buildUI(const std::vector<ModuleParameter>& parameters,
                 const ParamMap& currentValues);

    /**
     * @brief Create widget for an int parameter
     */
    QWidget* createIntWidget(const ModuleParameter& param,
                             const ParamMap& currentValues);

    /**
     * @brief Create widget for a float parameter
     */
    QWidget* createFloatWidget(const ModuleParameter& param,
                               const ParamMap& currentValues);

    /**
     * @brief Create widget for a string parameter
     */
    QWidget* createStringWidget(const ModuleParameter& param,
                                const ParamMap& currentValues);

    /**
     * @brief Create widget for a bool parameter
     */
    QWidget* createBoolWidget(const ModuleParameter& param,
                              const ParamMap& currentValues);

    // Layouts
    QFormLayout* formLayout_;
    QDialogButtonBox* buttonBox_;

    // Map to store widgets for each parameter
    std::map<std::string, QWidget*> parameterWidgets_;

    // Store parameter info for validation
    std::vector<ModuleParameter> parameters_;
};


#endif // MODULEPARAMETERDIALOG_HPP
