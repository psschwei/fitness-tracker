#include "bodycomposition.h"
#include <QDebug>

BodyComposition::BodyComposition()
    : m_weight(0.0)
    , m_waistCircumference(0.0)
    , m_height(0.0)
    , m_neckCircumference(0.0)
    , m_timestamp(QDateTime::currentDateTime())
{
}

BodyComposition::BodyComposition(const QDate &date, double weight, double waistCircumference, double height, double neckCircumference, const QString &notes)
    : m_date(date)
    , m_weight(weight)
    , m_waistCircumference(waistCircumference)
    , m_height(height)
    , m_neckCircumference(neckCircumference)
    , m_notes(notes)
    , m_timestamp(QDateTime::currentDateTime())
{
}

bool BodyComposition::isValid() const
{
    return validationErrors().isEmpty();
}

QStringList BodyComposition::validationErrors() const
{
    QStringList errors;
    
    if (!m_date.isValid()) {
        errors << "Invalid date";
    }
    
    if (m_weight <= 0.0) {
        errors << "Weight must be greater than 0";
    } else if (m_weight > 1000.0) {
        errors << "Weight seems unrealistic (over 1000)";
    }
    
    if (m_waistCircumference <= 0.0) {
        errors << "Waist circumference must be greater than 0";
    } else if (m_waistCircumference > 200.0) {
        errors << "Waist circumference seems unrealistic (over 200)";
    }

    if (m_height <= 0.0) {
        errors << "Height must be greater than 0";
    } else if (m_height > 300.0) {
        errors << "Height seems unrealistic (over 300 cm)";
    }

    if (m_neckCircumference <= 0.0) {
        errors << "Neck circumference must be greater than 0";
    } else if (m_neckCircumference > 100.0) {
        errors << "Neck circumference seems unrealistic (over 100)";
    }
    
    return errors;
}

bool BodyComposition::isEmpty() const
{
    return m_weight <= 0.0 && m_waistCircumference <= 0.0 && m_height <= 0.0 && m_neckCircumference <= 0.0 && m_notes.isEmpty();
} 