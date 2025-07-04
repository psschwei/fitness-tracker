#include "exercise.h"
#include <QDebug>

Exercise::Exercise()
    : m_id(0)
    , m_isActive(true)
    , m_createdAt(QDateTime::currentDateTime())
    , m_updatedAt(QDateTime::currentDateTime())
{
}

Exercise::Exercise(int id, const QString &name, const QString &category, bool isActive)
    : m_id(id)
    , m_name(name)
    , m_category(category)
    , m_isActive(isActive)
    , m_createdAt(QDateTime::currentDateTime())
    , m_updatedAt(QDateTime::currentDateTime())
{
}

bool Exercise::isValid() const
{
    return validationErrors().isEmpty();
}

QStringList Exercise::validationErrors() const
{
    QStringList errors;
    
    if (m_name.trimmed().isEmpty()) {
        errors << "Exercise name cannot be empty";
    } else if (m_name.length() > 255) {
        errors << "Exercise name is too long (max 255 characters)";
    }
    
    if (m_category.trimmed().isEmpty()) {
        errors << "Exercise category cannot be empty";
    }
    
    return errors;
}

QJsonObject Exercise::toJson() const
{
    QJsonObject json;
    json["id"] = m_id;
    json["name"] = m_name;
    json["category"] = m_category;
    json["isActive"] = m_isActive;
    json["createdAt"] = m_createdAt.toString(Qt::ISODate);
    json["updatedAt"] = m_updatedAt.toString(Qt::ISODate);
    return json;
}

Exercise Exercise::fromJson(const QJsonObject &json)
{
    Exercise exercise;
    
    if (json.contains("id")) {
        exercise.m_id = json["id"].toInt();
    }
    
    if (json.contains("name")) {
        exercise.m_name = json["name"].toString();
    }
    
    if (json.contains("category")) {
        exercise.m_category = json["category"].toString();
    }
    
    if (json.contains("isActive")) {
        exercise.m_isActive = json["isActive"].toBool();
    }
    
    if (json.contains("createdAt")) {
        exercise.m_createdAt = QDateTime::fromString(json["createdAt"].toString(), Qt::ISODate);
    } else {
        exercise.m_createdAt = QDateTime::currentDateTime();
    }
    
    if (json.contains("updatedAt")) {
        exercise.m_updatedAt = QDateTime::fromString(json["updatedAt"].toString(), Qt::ISODate);
    } else {
        exercise.m_updatedAt = QDateTime::currentDateTime();
    }
    
    return exercise;
} 