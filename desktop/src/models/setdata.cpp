#include "setdata.h"
#include <QDebug>

SetData::SetData()
    : m_weight(0.0)
    , m_reps(0)
    , m_sets(1)
{
}

SetData::SetData(double weight, int reps, int sets)
    : m_weight(weight)
    , m_reps(reps)
    , m_sets(sets)
{
}

bool SetData::isValid() const
{
    return validationErrors().isEmpty();
}

QStringList SetData::validationErrors() const
{
    QStringList errors;
    
    if (m_weight < 0.0) {
        errors << "Weight cannot be negative";
    }
    
    if (m_reps <= 0) {
        errors << "Reps must be greater than 0";
    }
    
    if (m_sets <= 0) {
        errors << "Sets must be greater than 0";
    }
    
    return errors;
}

QJsonObject SetData::toJson() const
{
    QJsonObject json;
    json["weight"] = m_weight;
    json["reps"] = m_reps;
    json["sets"] = m_sets;
    return json;
}

SetData SetData::fromJson(const QJsonObject &json)
{
    SetData setData;
    
    if (json.contains("weight")) {
        setData.m_weight = json["weight"].toDouble();
    }
    
    if (json.contains("reps")) {
        setData.m_reps = json["reps"].toInt();
    }
    
    if (json.contains("sets")) {
        setData.m_sets = json["sets"].toInt();
    }
    
    return setData;
} 