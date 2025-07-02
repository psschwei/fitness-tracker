#ifndef BODYCOMPOSITIONFORM_H
#define BODYCOMPOSITIONFORM_H

#include <QWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QLabel>
#include <QDate>
#include "models/bodycomposition.h"

class BodyCompositionForm : public QWidget
{
    Q_OBJECT

public:
    explicit BodyCompositionForm(QWidget *parent = nullptr);
    
    void setDate(const QDate &date);
    BodyComposition getData() const;
    void setData(const BodyComposition &data);
    void clear();

signals:
    void dataSaved(const BodyComposition &data);
    void cancelled();

private slots:
    void onSaveClicked();
    void onCancelClicked();
    void validateForm();

private:
    void setupUI();
    void updateValidationDisplay();
    
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
    
    // Validation state
    QStringList m_validationErrors;
};

#endif // BODYCOMPOSITIONFORM_H 