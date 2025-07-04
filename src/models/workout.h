#ifndef WORKOUT_H
#define WORKOUT_H

#include <QDate>
#include <QString>
#include <QDateTime>
#include <QJsonObject>
#include <QJsonArray>
#include <QList>
#include "setdata.h"

class WorkoutExercise;

class Workout
{
public:
    enum Status {
        InProgress,
        Completed,
        Cancelled
    };
    
    Workout();
    Workout(int id, const QDate &date, const QString &notes = QString(), Status status = InProgress);
    
    // Getters
    int id() const { return m_id; }
    QDate date() const { return m_date; }
    QString notes() const { return m_notes; }
    Status status() const { return m_status; }
    QDateTime createdAt() const { return m_createdAt; }
    QDateTime updatedAt() const { return m_updatedAt; }
    QList<WorkoutExercise> exercises() const { return m_exercises; }
    
    // Setters
    void setId(int id) { m_id = id; }
    void setDate(const QDate &date) { m_date = date; }
    void setNotes(const QString &notes) { m_notes = notes; }
    void setStatus(Status status) { m_status = status; }
    void setExercises(const QList<WorkoutExercise> &exercises) { m_exercises = exercises; }
    
    // Exercise management
    void addExercise(const WorkoutExercise &exercise);
    void removeExercise(int index);
    void clearExercises();
    
    // Validation
    bool isValid() const;
    QStringList validationErrors() const;
    
    // Utility
    bool isEmpty() const;
    QString statusString() const;
    static Status statusFromString(const QString &status);
    
    // Serialization
    QJsonObject toJson() const;
    static Workout fromJson(const QJsonObject &json);

private:
    int m_id;
    QDate m_date;
    QString m_notes;
    Status m_status;
    QDateTime m_createdAt;
    QDateTime m_updatedAt;
    QList<WorkoutExercise> m_exercises;
};

class WorkoutExercise
{
public:
    WorkoutExercise();
    WorkoutExercise(int id, int workoutId, int exerciseId, const QString &exerciseName, 
                   const QList<SetData> &setsData, const QString &notes = QString());
    
    // Getters
    int id() const { return m_id; }
    int workoutId() const { return m_workoutId; }
    int exerciseId() const { return m_exerciseId; }
    QString exerciseName() const { return m_exerciseName; }
    QList<SetData> setsData() const { return m_setsData; }
    QString notes() const { return m_notes; }
    QDateTime createdAt() const { return m_createdAt; }
    QDateTime updatedAt() const { return m_updatedAt; }
    
    // Setters
    void setId(int id) { m_id = id; }
    void setWorkoutId(int workoutId) { m_workoutId = workoutId; }
    void setExerciseId(int exerciseId) { m_exerciseId = exerciseId; }
    void setExerciseName(const QString &name) { m_exerciseName = name; }
    void setSetsData(const QList<SetData> &setsData) { m_setsData = setsData; }
    void setNotes(const QString &notes) { m_notes = notes; }
    
    // Set management
    void addSet(const SetData &set);
    void removeSet(int index);
    void clearSets();
    
    // Validation
    bool isValid() const;
    QStringList validationErrors() const;
    
    // Serialization
    QJsonObject toJson() const;
    static WorkoutExercise fromJson(const QJsonObject &json);

private:
    int m_id;
    int m_workoutId;
    int m_exerciseId;
    QString m_exerciseName;
    QList<SetData> m_setsData;
    QString m_notes;
    QDateTime m_createdAt;
    QDateTime m_updatedAt;
};

#endif // WORKOUT_H 