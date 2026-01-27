#include "ModuleParameterDialog.hpp"
#include <QVBoxLayout>
#include <QGroupBox>
#include <QScrollArea>

ModuleParameterDialog::ModuleParameterDialog(const std::string& moduleName,
                                             const std::vector<ModuleParameter>& parameters,
                                             const ParamMap& currentValues,
                                             QWidget *parent)
    : QDialog(parent)
    , parameters_(parameters)
{
    setWindowTitle(QString("Configure: %1").arg(QString::fromStdString(moduleName)));
    setMinimumWidth(400);

    // Main layout
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // Title label
    QLabel* titleLabel = new QLabel(QString("Configure parameters for <b>%1</b>:")
                                        .arg(QString::fromStdString(moduleName)));
    mainLayout->addWidget(titleLabel);

    // Create scroll area for parameters (in case there are many)
    QScrollArea* scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);

    QWidget* scrollWidget = new QWidget();
    formLayout_ = new QFormLayout(scrollWidget);
    formLayout_->setLabelAlignment(Qt::AlignRight);

    scrollArea->setWidget(scrollWidget);
    mainLayout->addWidget(scrollArea);

    // Build the form
    buildUI(parameters, currentValues);

    // Button box
    buttonBox_ = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttonBox_, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox_, &QDialogButtonBox::rejected, this, &QDialog::reject);
    mainLayout->addWidget(buttonBox_);
}

ModuleParameterDialog::~ModuleParameterDialog()
{
    // Widgets are automatically deleted by Qt's parent-child mechanism
}

ParamMap ModuleParameterDialog::getParameters() const
{
    ParamMap result;

    for (const auto& param : parameters_) {
        auto it = parameterWidgets_.find(param.name);
        if (it == parameterWidgets_.end()) {
            continue; // Should not happen
        }

        QWidget* widget = it->second;

        if (param.type == "int") {
            QSpinBox* spinBox = qobject_cast<QSpinBox*>(widget);
            if (spinBox) {
                result[param.name] = spinBox->value();
            }
        }
        else if (param.type == "float") {
            QDoubleSpinBox* doubleSpinBox = qobject_cast<QDoubleSpinBox*>(widget);
            if (doubleSpinBox) {
                result[param.name] = static_cast<float>(doubleSpinBox->value());
            }
        }
        else if (param.type == "string") {
            QLineEdit* lineEdit = qobject_cast<QLineEdit*>(widget);
            if (lineEdit) {
                result[param.name] = lineEdit->text().toStdString();
            }
        }
        else if (param.type == "bool") {
            QCheckBox* checkBox = qobject_cast<QCheckBox*>(widget);
            if (checkBox) {
                result[param.name] = checkBox->isChecked();
            }
        }
    }

    return result;
}

void ModuleParameterDialog::buildUI(const std::vector<ModuleParameter>& parameters,
                                    const ParamMap& currentValues)
{
    for (const auto& param : parameters) {
        QWidget* widget = nullptr;

        if (param.type == "int") {
            widget = createIntWidget(param, currentValues);
        }
        else if (param.type == "float") {
            widget = createFloatWidget(param, currentValues);
        }
        else if (param.type == "string") {
            widget = createStringWidget(param, currentValues);
        }
        else if (param.type == "bool") {
            widget = createBoolWidget(param, currentValues);
        }

        if (widget) {
            QString label = QString::fromStdString(param.name) + ":";
            formLayout_->addRow(label, widget);
            parameterWidgets_[param.name] = widget;

            // Add tooltip if description is provided
            if (!param.description.empty()) {
                widget->setToolTip(QString::fromStdString(param.description));
            }
        }
    }
}

QWidget* ModuleParameterDialog::createIntWidget(const ModuleParameter& param,
                                                const ParamMap& currentValues)
{
    QSpinBox* spinBox = new QSpinBox();

    // Set range
    if (param.minInt.has_value()) {
        spinBox->setMinimum(param.minInt.value());
    } else {
        spinBox->setMinimum(std::numeric_limits<int>::min());
    }

    if (param.maxInt.has_value()) {
        spinBox->setMaximum(param.maxInt.value());
    } else {
        spinBox->setMaximum(std::numeric_limits<int>::max());
    }

    // Set current value
    auto it = currentValues.find(param.name);
    if (it != currentValues.end() && std::holds_alternative<int>(it->second)) {
        spinBox->setValue(std::get<int>(it->second));
    } else if (std::holds_alternative<int>(param.defaultValue)) {
        spinBox->setValue(std::get<int>(param.defaultValue));
    }

    return spinBox;
}

QWidget* ModuleParameterDialog::createFloatWidget(const ModuleParameter& param,
                                                  const ParamMap& currentValues)
{
    QDoubleSpinBox* doubleSpinBox = new QDoubleSpinBox();
    doubleSpinBox->setDecimals(3);

    // Set range
    if (param.minFloat.has_value()) {
        doubleSpinBox->setMinimum(param.minFloat.value());
    } else {
        doubleSpinBox->setMinimum(-1e10);
    }

    if (param.maxFloat.has_value()) {
        doubleSpinBox->setMaximum(param.maxFloat.value());
    } else {
        doubleSpinBox->setMaximum(1e10);
    }

    // Set current value
    auto it = currentValues.find(param.name);
    if (it != currentValues.end() && std::holds_alternative<float>(it->second)) {
        doubleSpinBox->setValue(std::get<float>(it->second));
    } else if (std::holds_alternative<float>(param.defaultValue)) {
        doubleSpinBox->setValue(std::get<float>(param.defaultValue));
    }

    return doubleSpinBox;
}

QWidget* ModuleParameterDialog::createStringWidget(const ModuleParameter& param,
                                                   const ParamMap& currentValues)
{
    QLineEdit* lineEdit = new QLineEdit();

    // Set current value
    auto it = currentValues.find(param.name);
    if (it != currentValues.end() && std::holds_alternative<std::string>(it->second)) {
        lineEdit->setText(QString::fromStdString(std::get<std::string>(it->second)));
    } else if (std::holds_alternative<std::string>(param.defaultValue)) {
        lineEdit->setText(QString::fromStdString(std::get<std::string>(param.defaultValue)));
    }

    return lineEdit;
}

QWidget* ModuleParameterDialog::createBoolWidget(const ModuleParameter& param,
                                                 const ParamMap& currentValues)
{
    QCheckBox* checkBox = new QCheckBox();

    // Set current value
    auto it = currentValues.find(param.name);
    if (it != currentValues.end() && std::holds_alternative<bool>(it->second)) {
        checkBox->setChecked(std::get<bool>(it->second));
    } else if (std::holds_alternative<bool>(param.defaultValue)) {
        checkBox->setChecked(std::get<bool>(param.defaultValue));
    }

    return checkBox;
}
