#include "bodycomposition.h"
#include <QDebug>
#include <QJsonDocument>

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

QJsonObject BodyComposition::toJson() const
{
    QJsonObject json;
    json["date"] = m_date.toString(Qt::ISODate);
    json["weight"] = m_weight;
    json["waistCircumference"] = m_waistCircumference;
    json["height"] = m_height;
    json["neckCircumference"] = m_neckCircumference;
    json["notes"] = m_notes;
    json["timestamp"] = m_timestamp.toString(Qt::ISODate);
    return json;
}

BodyComposition BodyComposition::fromJson(const QJsonObject &json)
{
    BodyComposition composition;
    
    if (json.contains("date")) {
        composition.m_date = QDate::fromString(json["date"].toString(), Qt::ISODate);
    }
    
    if (json.contains("weight")) {
        composition.m_weight = json["weight"].toDouble();
    }
    
    if (json.contains("waistCircumference")) {
        composition.m_waistCircumference = json["waistCircumference"].toDouble();
    }
    
    if (json.contains("height")) {
        composition.m_height = json["height"].toDouble();
    }
    
    if (json.contains("neckCircumference")) {
        composition.m_neckCircumference = json["neckCircumference"].toDouble();
    }
    
    if (json.contains("notes")) {
        composition.m_notes = json["notes"].toString();
    }
    
    if (json.contains("timestamp")) {
        composition.m_timestamp = QDateTime::fromString(json["timestamp"].toString(), Qt::ISODate);
    } else {
        composition.m_timestamp = QDateTime::currentDateTime();
    }
    
    return composition;
} 