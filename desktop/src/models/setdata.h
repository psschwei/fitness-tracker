#ifndef SETDATA_H
#define SETDATA_H

#include <QJsonObject>

class SetData
{
public:
    SetData();
    SetData(double weight, int reps, int sets = 1);
    
    // Getters
    double weight() const { return m_weight; }
    int reps() const { return m_reps; }
    int sets() const { return m_sets; }
    
    // Setters
    void setWeight(double weight) { m_weight = weight; }
    void setReps(int reps) { m_reps = reps; }
    void setSets(int sets) { m_sets = sets; }
    
    // Validation
    bool isValid() const;
    QStringList validationErrors() const;
    
    // Serialization
    QJsonObject toJson() const;
    static SetData fromJson(const QJsonObject &json);

private:
    double m_weight;
    int m_reps;
    int m_sets;
};

#endif // SETDATA_H 