#include "bodycompositionform.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QDoubleValidator>
#include <QMessageBox>

BodyCompositionForm::BodyCompositionForm(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
    
    // Connect validation signals
    connect(m_weightEdit, &QLineEdit::textChanged, this, &BodyCompositionForm::validateForm);
    connect(m_waistEdit, &QLineEdit::textChanged, this, &BodyCompositionForm::validateForm);
    connect(m_heightEdit, &QLineEdit::textChanged, this, &BodyCompositionForm::validateForm);
    connect(m_neckEdit, &QLineEdit::textChanged, this, &BodyCompositionForm::validateForm);
    
    // Connect button signals
    connect(m_saveButton, &QPushButton::clicked, this, &BodyCompositionForm::onSaveClicked);
    connect(m_cancelButton, &QPushButton::clicked, this, &BodyCompositionForm::onCancelClicked);
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
    
    m_saveButton = new QPushButton("Save", this);
    m_cancelButton = new QPushButton("Cancel", this);
    
    m_validationLabel = new QLabel(this);
    m_validationLabel->setStyleSheet("QLabel { color: red; }");
    m_validationLabel->setWordWrap(true);
    m_validationLabel->hide();
    
    // Create form layout
    QGroupBox *formGroup = new QGroupBox("Body Composition", this);
    QGridLayout *formLayout = new QGridLayout(formGroup);
    
    formLayout->addWidget(new QLabel("Weight (lbs):"), 0, 0);
    formLayout->addWidget(m_weightEdit, 0, 1);
    
    formLayout->addWidget(new QLabel("Waist (inches):"), 1, 0);
    formLayout->addWidget(m_waistEdit, 1, 1);

    formLayout->addWidget(new QLabel("Height (inches):"), 2, 0);
    formLayout->addWidget(m_heightEdit, 2, 1);

    formLayout->addWidget(new QLabel("Neck (inches):"), 3, 0);
    formLayout->addWidget(m_neckEdit, 3, 1);
    
    formLayout->addWidget(new QLabel("Notes:"), 4, 0);
    formLayout->addWidget(m_notesEdit, 4, 1);
    
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
    return BodyComposition(
        m_currentDate,
        m_weightEdit->text().toDouble(),
        m_waistEdit->text().toDouble(),
        m_heightEdit->text().toDouble(),
        m_neckEdit->text().toDouble(),
        m_notesEdit->toPlainText()
    );
}

void BodyCompositionForm::setData(const BodyComposition &data)
{
    m_weightEdit->setText(QString::number(data.weight(), 'f', 1));
    m_waistEdit->setText(QString::number(data.waistCircumference(), 'f', 1));
    m_heightEdit->setText(QString::number(data.height(), 'f', 1));
    m_neckEdit->setText(QString::number(data.neckCircumference(), 'f', 1));
    m_notesEdit->setPlainText(data.notes());
}

void BodyCompositionForm::clear()
{
    m_weightEdit->clear();
    m_waistEdit->clear();
    m_heightEdit->clear();
    m_neckEdit->clear();
    m_notesEdit->clear();
    m_validationLabel->hide();
}

void BodyCompositionForm::validateForm()
{
    BodyComposition data = getData();
    m_validationErrors = data.validationErrors();
    
    updateValidationDisplay();
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