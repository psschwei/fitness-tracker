#include "bodycompositionform.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QDoubleValidator>
#include <QMessageBox>
#include <QShortcut>
#include "utils/calculations.h"

BodyCompositionForm::BodyCompositionForm(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
    
    // Connect validation signals
    connect(m_weightEdit, &QLineEdit::textChanged, this, &BodyCompositionForm::validateForm);
    connect(m_waistEdit, &QLineEdit::textChanged, this, &BodyCompositionForm::validateForm);
    connect(m_heightEdit, &QLineEdit::textChanged, this, &BodyCompositionForm::validateForm);
    connect(m_neckEdit, &QLineEdit::textChanged, this, &BodyCompositionForm::validateForm);
    
    // Connect calculation signals
    connect(m_weightEdit, &QLineEdit::textChanged, this, &BodyCompositionForm::updateCalculations);
    connect(m_waistEdit, &QLineEdit::textChanged, this, &BodyCompositionForm::updateCalculations);
    connect(m_heightEdit, &QLineEdit::textChanged, this, &BodyCompositionForm::updateCalculations);
    connect(m_neckEdit, &QLineEdit::textChanged, this, &BodyCompositionForm::updateCalculations);
    connect(m_genderGroup, &QButtonGroup::buttonClicked, this, &BodyCompositionForm::updateCalculations);
    
    // Connect button signals
    connect(m_saveButton, &QPushButton::clicked, this, &BodyCompositionForm::onSaveClicked);
    connect(m_cancelButton, &QPushButton::clicked, this, &BodyCompositionForm::onCancelClicked);
    
    // Setup keyboard shortcuts
    QShortcut *saveShortcut = new QShortcut(QKeySequence(Qt::Key_Return), this);
    connect(saveShortcut, &QShortcut::activated, this, &BodyCompositionForm::onSaveClicked);
    
    QShortcut *cancelShortcut = new QShortcut(QKeySequence(Qt::Key_Escape), this);
    connect(cancelShortcut, &QShortcut::activated, this, &BodyCompositionForm::onCancelClicked);
}

void BodyCompositionForm::setupUI()
{
    // Create form widgets
    m_weightEdit = new QLineEdit(this);
    m_weightEdit->setPlaceholderText("Enter weight");
    m_weightEdit->setValidator(new QDoubleValidator(0.1, 1000.0, 1, this));
    
    m_waistEdit = new QLineEdit(this);
    m_waistEdit->setPlaceholderText("Enter waist circumference");
    m_waistEdit->setValidator(new QDoubleValidator(0.1, 200.0, 1, this));

    m_heightEdit = new QLineEdit(this);
    m_heightEdit->setPlaceholderText("Enter height (inches)");
    m_heightEdit->setValidator(new QDoubleValidator(12.0, 120.0, 1, this));

    m_neckEdit = new QLineEdit(this);
    m_neckEdit->setPlaceholderText("Enter neck circumference (inches)");
    m_neckEdit->setValidator(new QDoubleValidator(5.0, 30.0, 1, this));
    
    m_notesEdit = new QTextEdit(this);
    m_notesEdit->setPlaceholderText("Optional notes...");
    m_notesEdit->setMaximumHeight(100);
    
    // Create gender selection
    m_genderGroup = new QButtonGroup(this);
    m_maleRadio = new QRadioButton("Male", this);
    m_femaleRadio = new QRadioButton("Female", this);
    m_maleRadio->setChecked(true); // Default to male
    m_genderGroup->addButton(m_maleRadio);
    m_genderGroup->addButton(m_femaleRadio);
    
    // Create calculation display labels
    m_bmiLabel = new QLabel("BMI: Not available", this);
    m_bmiLabel->setStyleSheet("QLabel { font-size: 14px; color: #007bff; padding: 5px; }");
    
    m_bodyFatLabel = new QLabel("Body Fat: Not available", this);
    m_bodyFatLabel->setStyleSheet("QLabel { font-size: 14px; color: #28a745; padding: 5px; }");
    
    m_saveButton = new QPushButton("Save", this);
    m_cancelButton = new QPushButton("Cancel", this);
    
    m_validationLabel = new QLabel(this);
    m_validationLabel->setStyleSheet("QLabel { color: red; }");
    m_validationLabel->setWordWrap(true);
    m_validationLabel->hide();
    
    // Create form layout with measurements on left, calculations on right
    QGroupBox *formGroup = new QGroupBox("Body Composition", this);
    QHBoxLayout *formLayout = new QHBoxLayout(formGroup);
    
    // Left side - Measurements
    QGridLayout *measurementsLayout = new QGridLayout;
    
    measurementsLayout->addWidget(new QLabel("Weight (lbs):"), 0, 0);
    measurementsLayout->addWidget(m_weightEdit, 0, 1);
    
    measurementsLayout->addWidget(new QLabel("Waist (inches):"), 1, 0);
    measurementsLayout->addWidget(m_waistEdit, 1, 1);

    measurementsLayout->addWidget(new QLabel("Height (inches):"), 2, 0);
    measurementsLayout->addWidget(m_heightEdit, 2, 1);

    measurementsLayout->addWidget(new QLabel("Neck (inches):"), 3, 0);
    measurementsLayout->addWidget(m_neckEdit, 3, 1);
    
    measurementsLayout->addWidget(new QLabel("Gender:"), 4, 0);
    QHBoxLayout *genderLayout = new QHBoxLayout;
    genderLayout->addWidget(m_maleRadio);
    genderLayout->addWidget(m_femaleRadio);
    genderLayout->addStretch();
    measurementsLayout->addLayout(genderLayout, 4, 1);
    
    measurementsLayout->addWidget(new QLabel("Notes:"), 5, 0);
    measurementsLayout->addWidget(m_notesEdit, 5, 1);
    
    // Right side - Calculations
    QVBoxLayout *calculationsLayout = new QVBoxLayout;
    QLabel *calcHeader = new QLabel("Calculations");
    calcHeader->setAlignment(Qt::AlignRight);
    calculationsLayout->addWidget(calcHeader);
    m_bmiLabel->setAlignment(Qt::AlignRight);
    calculationsLayout->addWidget(m_bmiLabel);
    m_bodyFatLabel->setAlignment(Qt::AlignRight);
    calculationsLayout->addWidget(m_bodyFatLabel);
    calculationsLayout->addStretch();
    
    // Add both layouts to the main form layout
    formLayout->addLayout(measurementsLayout);
    formLayout->addSpacing(20); // Add some spacing between sections
    formLayout->addLayout(calculationsLayout);
    
    // Create button layout
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_cancelButton);
    buttonLayout->addWidget(m_saveButton);
    
    // Create main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(40, 20, 40, 20);
    mainLayout->addWidget(formGroup);
    mainLayout->addWidget(m_validationLabel);
    mainLayout->addStretch();
    mainLayout->addLayout(buttonLayout);
    
    setLayout(mainLayout);
}

void BodyCompositionForm::setDate(const QDate &date)
{
    m_currentDate = date;
}

BodyComposition BodyCompositionForm::getData() const
{
    BodyComposition data(
        m_currentDate,
        m_weightEdit->text().toDouble(),
        m_waistEdit->text().toDouble(),
        m_heightEdit->text().toDouble(),
        m_neckEdit->text().toDouble(),
        m_notesEdit->toPlainText(),
        m_maleRadio->isChecked()
    );
    return data;
}

void BodyCompositionForm::setData(const BodyComposition &data)
{
    m_weightEdit->setText(QString::number(data.weight(), 'f', 1));
    m_waistEdit->setText(QString::number(data.waistCircumference(), 'f', 1));
    m_heightEdit->setText(QString::number(data.height(), 'f', 1));
    m_neckEdit->setText(QString::number(data.neckCircumference(), 'f', 1));
    m_notesEdit->setPlainText(data.notes());
    
    if (data.isMale()) {
        m_maleRadio->setChecked(true);
    } else {
        m_femaleRadio->setChecked(true);
    }
    
    updateCalculations();
}

void BodyCompositionForm::prefillWithData(const BodyComposition &data)
{
    // Only prefill if the data is not empty
    if (!data.isEmpty()) {
        if (data.weight() > 0.0) {
            m_weightEdit->setText(QString::number(data.weight(), 'f', 1));
        }
        if (data.waistCircumference() > 0.0) {
            m_waistEdit->setText(QString::number(data.waistCircumference(), 'f', 1));
        }
        if (data.height() > 0.0) {
            m_heightEdit->setText(QString::number(data.height(), 'f', 1));
        }
        if (data.neckCircumference() > 0.0) {
            m_neckEdit->setText(QString::number(data.neckCircumference(), 'f', 1));
        }
        // Don't prefill notes as they're usually day-specific
        
        // Set gender from previous data
        if (data.isMale()) {
            m_maleRadio->setChecked(true);
        } else {
            m_femaleRadio->setChecked(true);
        }
    }
}

void BodyCompositionForm::clear()
{
    m_weightEdit->clear();
    m_waistEdit->clear();
    m_heightEdit->clear();
    m_neckEdit->clear();
    m_notesEdit->clear();
    m_maleRadio->setChecked(true); // Reset to male
    m_validationLabel->hide();
    updateCalculations();
}

void BodyCompositionForm::validateForm()
{
    BodyComposition data = getData();
    m_validationErrors = data.validationErrors();
    
    updateValidationDisplay();
}

void BodyCompositionForm::updateCalculations()
{
    updateCalculationDisplay();
}

void BodyCompositionForm::updateValidationDisplay()
{
    if (m_validationErrors.isEmpty()) {
        m_validationLabel->hide();
        m_saveButton->setEnabled(true);
    } else {
        m_validationLabel->setText("Errors:\n• " + m_validationErrors.join("\n• "));
        m_validationLabel->show();
        m_saveButton->setEnabled(false);
    }
}

void BodyCompositionForm::updateCalculationDisplay()
{
    BodyComposition data = getData();
    
    // Update BMI display
    QString bmiText = Calculations::formatBMI(data.bmi());
    m_bmiLabel->setText(QString("BMI: %1").arg(bmiText));
    m_bmiLabel->setStyleSheet("QLabel { font-size: 14px; padding: 5px; }");
    
    // Update body fat display
    QString bodyFatText = Calculations::formatBodyFat(data.bodyFatPercentage(), data.isMale());
    m_bodyFatLabel->setText(QString("Body Fat: %1").arg(bodyFatText));
    m_bodyFatLabel->setStyleSheet("QLabel { font-size: 14px; padding: 5px; }");
}

void BodyCompositionForm::onSaveClicked()
{
    BodyComposition data = getData();
    
    if (!data.isValid()) {
        QMessageBox::warning(this, "Validation Error", 
                           "Please fix the validation errors before saving.");
        return;
    }
    
    emit dataSaved(data);
}

void BodyCompositionForm::onCancelClicked()
{
    emit cancelled();
} 