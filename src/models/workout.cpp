#include "workout.h"
#include <QDebug>

// Workout implementation
Workout::Workout()
    : m_id(0)
    , m_status(InProgress)
    , m_createdAt(QDateTime::currentDateTime())
    , m_updatedAt(QDateTime::currentDateTime())
{
}

Workout::Workout(int id, const QDate &date, const QString &notes, Status status)
    : m_id(id)
    , m_date(date)
    , m_notes(notes)
    , m_status(status)
    , m_createdAt(QDateTime::currentDateTime())
    , m_updatedAt(QDateTime::currentDateTime())
{
}

void Workout::addExercise(const WorkoutExercise &exercise)
{
    m_exercises.append(exercise);
}

void Workout::removeExercise(int index)
{
    if (index >= 0 && index < m_exercises.size()) {
        m_exercises.removeAt(index);
    }
}

void Workout::clearExercises()
{
    m_exercises.clear();
}

bool Workout::isValid() const
{
    return validationErrors().isEmpty();
}

QStringList Workout::validationErrors() const
{
    QStringList errors;
    
    if (!m_date.isValid()) {
        errors << "Invalid workout date";
    }
    
    return errors;
}

bool Workout::isEmpty() const
{
    return m_exercises.isEmpty();
}

QString Workout::statusString() const
{
    switch (m_status) {
        case InProgress: return "in_progress";
        case Completed: return "completed";
        case Cancelled: return "cancelled";
        default: return "unknown";
    }
}

Workout::Status Workout::statusFromString(const QString &status)
{
    if (status == "completed") return Completed;
    if (status == "cancelled") return Cancelled;
    return InProgress; // default
}

QJsonObject Workout::toJson() const
{
    QJsonObject json;
    json["id"] = m_id;
    json["date"] = m_date.toString(Qt::ISODate);
    json["notes"] = m_notes;
    json["status"] = statusString();
    json["createdAt"] = m_createdAt.toString(Qt::ISODate);
    json["updatedAt"] = m_updatedAt.toString(Qt::ISODate);
    
    QJsonArray exercisesArray;
    for (const WorkoutExercise &exercise : m_exercises) {
        exercisesArray.append(exercise.toJson());
    }
    json["exercises"] = exercisesArray;
    
    return json;
}

Workout Workout::fromJson(const QJsonObject &json)
{
    Workout workout;
    
    if (json.contains("id")) {
        workout.m_id = json["id"].toInt();
    }
    
    if (json.contains("date")) {
        workout.m_date = QDate::fromString(json["date"].toString(), Qt::ISODate);
    }
    
    if (json.contains("notes")) {
        workout.m_notes = json["notes"].toString();
    }
    
    if (json.contains("status")) {
        workout.m_status = statusFromString(json["status"].toString());
    }
    
    if (json.contains("createdAt")) {
        workout.m_createdAt = QDateTime::fromString(json["createdAt"].toString(), Qt::ISODate);
    } else {
        workout.m_createdAt = QDateTime::currentDateTime();
    }
    
    if (json.contains("updatedAt")) {
        workout.m_updatedAt = QDateTime::fromString(json["updatedAt"].toString(), Qt::ISODate);
    } else {
        workout.m_updatedAt = QDateTime::currentDateTime();
    }
    
    if (json.contains("exercises")) {
        QJsonArray exercisesArray = json["exercises"].toArray();
        for (const QJsonValue &value : exercisesArray) {
            WorkoutExercise exercise = WorkoutExercise::fromJson(value.toObject());
            workout.m_exercises.append(exercise);
        }
    }
    
    return workout;
}

// WorkoutExercise implementation
WorkoutExercise::WorkoutExercise()
    : m_id(0)
    , m_workoutId(0)
    , m_exerciseId(0)
    , m_createdAt(QDateTime::currentDateTime())
    , m_updatedAt(QDateTime::currentDateTime())
{
}

WorkoutExercise::WorkoutExercise(int id, int workoutId, int exerciseId, const QString &exerciseName, 
                               const QList<SetData> &setsData, const QString &notes)
    : m_id(id)
    , m_workoutId(workoutId)
    , m_exerciseId(exerciseId)
    , m_exerciseName(exerciseName)
    , m_setsData(setsData)
    , m_notes(notes)
    , m_createdAt(QDateTime::currentDateTime())
    , m_updatedAt(QDateTime::currentDateTime())
{
}

void WorkoutExercise::addSet(const SetData &set)
{
    m_setsData.append(set);
}

void WorkoutExercise::removeSet(int index)
{
    if (index >= 0 && index < m_setsData.size()) {
        m_setsData.removeAt(index);
    }
}

void WorkoutExercise::clearSets()
{
    m_setsData.clear();
}

bool WorkoutExercise::isValid() const
{
    return validationErrors().isEmpty();
}

QStringList WorkoutExercise::validationErrors() const
{
    QStringList errors;
    
    if (m_exerciseId <= 0) {
        errors << "Invalid exercise ID";
    }
    
    if (m_exerciseName.trimmed().isEmpty()) {
        errors << "Exercise name cannot be empty";
    }
    
    if (m_setsData.isEmpty()) {
        errors << "At least one set is required";
    } else {
        for (int i = 0; i < m_setsData.size(); ++i) {
            if (!m_setsData[i].isValid()) {
                errors << QString("Set %1 is invalid").arg(i + 1);
            }
        }
    }
    
    return errors;
}

QJsonObject WorkoutExercise::toJson() const
{
    QJsonObject json;
    json["id"] = m_id;
    json["workoutId"] = m_workoutId;
    json["exerciseId"] = m_exerciseId;
    json["exerciseName"] = m_exerciseName;
    json["notes"] = m_notes;
    json["createdAt"] = m_createdAt.toString(Qt::ISODate);
    json["updatedAt"] = m_updatedAt.toString(Qt::ISODate);
    
    QJsonArray setsArray;
    for (const SetData &set : m_setsData) {
        setsArray.append(set.toJson());
    }
    json["setsData"] = setsArray;
    
    return json;
}

WorkoutExercise WorkoutExercise::fromJson(const QJsonObject &json)
{
    WorkoutExercise exercise;
    
    if (json.contains("id")) {
        exercise.m_id = json["id"].toInt();
    }
    
    if (json.contains("workoutId")) {
        exercise.m_workoutId = json["workoutId"].toInt();
    }
    
    if (json.contains("exerciseId")) {
        exercise.m_exerciseId = json["exerciseId"].toInt();
    }
    
    if (json.contains("exerciseName")) {
        exercise.m_exerciseName = json["exerciseName"].toString();
    }
    
    if (json.contains("notes")) {
        exercise.m_notes = json["notes"].toString();
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
    
    if (json.contains("setsData")) {
        QJsonArray setsArray = json["setsData"].toArray();
        for (const QJsonValue &value : setsArray) {
            SetData set = SetData::fromJson(value.toObject());
            exercise.m_setsData.append(set);
        }
    }
    
    return exercise;
} 