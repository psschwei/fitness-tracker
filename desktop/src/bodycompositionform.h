#ifndef BODYCOMPOSITIONFORM_H
#define BODYCOMPOSITIONFORM_H

#include <QWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QLabel>
#include <QDate>
#include <QRadioButton>
#include <QButtonGroup>
#include "models/bodycomposition.h"

class BodyCompositionForm : public QWidget
{
    Q_OBJECT

public:
    explicit BodyCompositionForm(QWidget *parent = nullptr);
    
    void setDate(const QDate &date);
    BodyComposition getData() const;
    void setData(const BodyComposition &data);
    void prefillWithData(const BodyComposition &data);
    void clear();

signals:
    void dataSaved(const BodyComposition &data);
    void cancelled();

private slots:
    void onSaveClicked();
    void onCancelClicked();
    void validateForm();
    void updateCalculations();

private:
    void setupUI();
    void updateValidationDisplay();
    void updateCalculationDisplay();
    
    QDate m_currentDate;
    
    // UI Elements
    QLineEdit *m_weightEdit;
    QLineEdit *m_waistEdit;
    QLineEdit *m_heightEdit;
    QLineEdit *m_neckEdit;
    QTextEdit *m_notesEdit;
    QPushButton *m_saveButton;
    QPushButton *m_cancelButton;
    QLabel *m_validationLabel;
    
    // Gender selection
    QButtonGroup *m_genderGroup;
    QRadioButton *m_maleRadio;
    QRadioButton *m_femaleRadio;
    
    // Calculation display
    QLabel *m_bmiLabel;
    QLabel *m_bodyFatLabel;
    
    // Validation state
    QStringList m_validationErrors;
};

#endif // BODYCOMPOSITIONFORM_H 