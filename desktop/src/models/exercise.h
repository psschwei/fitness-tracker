#ifndef EXERCISE_H
#define EXERCISE_H

#include <QString>
#include <QDateTime>
#include <QJsonObject>

class Exercise
{
public:
    Exercise();
    Exercise(int id, const QString &name, const QString &category, bool isActive = true);
    
    // Getters
    int id() const { return m_id; }
    QString name() const { return m_name; }
    QString category() const { return m_category; }
    bool isActive() const { return m_isActive; }
    QDateTime createdAt() const { return m_createdAt; }
    QDateTime updatedAt() const { return m_updatedAt; }
    
    // Setters
    void setId(int id) { m_id = id; }
    void setName(const QString &name) { m_name = name; }
    void setCategory(const QString &category) { m_category = category; }
    void setIsActive(bool isActive) { m_isActive = isActive; }
    
    // Validation
    bool isValid() const;
    QStringList validationErrors() const;
    
    // Serialization
    QJsonObject toJson() const;
    static Exercise fromJson(const QJsonObject &json);

private:
    int m_id;
    QString m_name;
    QString m_category;
    bool m_isActive;
    QDateTime m_createdAt;
    QDateTime m_updatedAt;
};

#endif // EXERCISE_H 