#ifndef BODYCOMPOSITION_H
#define BODYCOMPOSITION_H

#include <QDate>
#include <QString>
#include <QDateTime>

class BodyComposition
{
public:
    BodyComposition();
    BodyComposition(const QDate &date, double weight, double waistCircumference, double height, double neckCircumference, const QString &notes = QString());
    
    // Getters
    QDate date() const { return m_date; }
    double weight() const { return m_weight; }
    double waistCircumference() const { return m_waistCircumference; }
    double height() const { return m_height; }
    double neckCircumference() const { return m_neckCircumference; }
    QString notes() const { return m_notes; }
    QDateTime timestamp() const { return m_timestamp; }
    
    // Setters
    void setDate(const QDate &date) { m_date = date; }
    void setWeight(double weight) { m_weight = weight; }
    void setWaistCircumference(double waistCircumference) { m_waistCircumference = waistCircumference; }
    void setHeight(double height) { m_height = height; }
    void setNeckCircumference(double neckCircumference) { m_neckCircumference = neckCircumference; }
    void setNotes(const QString &notes) { m_notes = notes; }
    
    // Validation
    bool isValid() const;
    QStringList validationErrors() const;
    
    // Utility
    bool isEmpty() const;

private:
    QDate m_date;
    double m_weight;
    double m_waistCircumference;
    double m_height;
    double m_neckCircumference;
    QString m_notes;
    QDateTime m_timestamp;
};

#endif // BODYCOMPOSITION_H 