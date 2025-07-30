#include "datamanager.h"
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QStandardPaths>
#include <QDebug>
#include <algorithm>
#include <limits>
#include <cstdio>

DataManager::DataManager(QObject *parent)
    : QObject(parent)
{
    m_dataFilePath = getDataFilePath();
    printf("DataManager: Looking for data file at: %s\n", m_dataFilePath.toLocal8Bit().data());
    ensureDataDirectory();
    loadData();
}

bool DataManager::saveBodyComposition(const BodyComposition &data)
{
    m_bodyCompositionData[data.date()] = data;
    bool success = saveData();
    if (success) {
        emit dataChanged();
    }
    return success;
}

BodyComposition DataManager::loadBodyComposition(const QDate &date)
{
    return m_bodyCompositionData.value(date, BodyComposition());
}

bool DataManager::hasBodyComposition(const QDate &date) const
{
    return m_bodyCompositionData.contains(date);
}

void DataManager::deleteBodyComposition(const QDate &date)
{
    if (m_bodyCompositionData.remove(date) > 0) {
        saveData();
        emit dataChanged();
    }
}

QList<QDate> DataManager::getBodyCompositionDates() const
{
    return m_bodyCompositionData.keys();
}

QList<BodyComposition> DataManager::getBodyCompositionRange(const QDate &start, const QDate &end) const
{
    QList<BodyComposition> result;
    for (auto it = m_bodyCompositionData.constBegin(); it != m_bodyCompositionData.constEnd(); ++it) {
        const QDate &date = it.key();
        if (date >= start && date <= end) {
            result.append(it.value());
        }
    }
    
    // Sort by date
    std::sort(result.begin(), result.end(), [](const BodyComposition &a, const BodyComposition &b) {
        return a.date() < b.date();
    });
    
    return result;
}

QList<BodyComposition> DataManager::getAllBodyCompositionSorted() const
{
    QList<BodyComposition> result = m_bodyCompositionData.values();
    
    // Sort by date
    std::sort(result.begin(), result.end(), [](const BodyComposition &a, const BodyComposition &b) {
        return a.date() < b.date();
    });
    
    return result;
}

QPair<double, double> DataManager::getWeightRange() const
{
    if (m_bodyCompositionData.isEmpty()) {
        return QPair<double, double>(0.0, 0.0);
    }
    
    double minWeight = std::numeric_limits<double>::max();
    double maxWeight = std::numeric_limits<double>::lowest();
    
    for (const BodyComposition &composition : m_bodyCompositionData.values()) {
        double weight = composition.weight();
        if (weight > 0) { // Only consider valid weights
            minWeight = qMin(minWeight, weight);
            maxWeight = qMax(maxWeight, weight);
        }
    }
    
    // If no valid weights found, return 0,0
    if (minWeight == std::numeric_limits<double>::max()) {
        return QPair<double, double>(0.0, 0.0);
    }
    
    return QPair<double, double>(minWeight, maxWeight);
}

QPair<QDate, QDate> DataManager::getDataDateRange() const
{
    if (m_bodyCompositionData.isEmpty()) {
        return QPair<QDate, QDate>(QDate(), QDate());
    }
    
    QList<QDate> dates = m_bodyCompositionData.keys();
    std::sort(dates.begin(), dates.end());
    
    return QPair<QDate, QDate>(dates.first(), dates.last());
}

bool DataManager::loadData()
{
    QFile file(m_dataFilePath);
    qDebug() << "DataManager::loadData: Checking file:" << m_dataFilePath;
    qDebug() << "DataManager::loadData: File exists:" << file.exists();
    if (!file.exists()) {
        qDebug() << "DataManager::loadData: No data file found, that's okay";
        return true; // No data file yet, that's okay
    }
    
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open data file for reading:" << m_dataFilePath;
        return false;
    }
    
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &error);
    if (error.error != QJsonParseError::NoError) {
        qWarning() << "Failed to parse data file:" << error.errorString();
        return false;
    }
    
    QJsonObject root = doc.object();
    if (root.contains("bodyComposition")) {
        QJsonArray array = root["bodyComposition"].toArray();
        m_bodyCompositionData.clear();
        qDebug() << "DataManager::loadData: Found" << array.size() << "body composition entries in JSON";
        
        for (const QJsonValue &value : array) {
            QJsonObject obj = value.toObject();
            BodyComposition composition = BodyComposition::fromJson(obj);
            if (composition.date().isValid()) {
                m_bodyCompositionData[composition.date()] = composition;
                qDebug() << "  Loaded entry for date:" << composition.date().toString() 
                         << "Weight:" << composition.weight() << "BMI:" << composition.bmi();
            } else {
                qDebug() << "  Skipped entry with invalid date:" << obj["date"].toString();
            }
        }
        qDebug() << "DataManager::loadData: Total loaded entries:" << m_bodyCompositionData.size();
    }
    
    // Load exercises
    if (root.contains("exercises")) {
        QJsonArray array = root["exercises"].toArray();
        m_exercises.clear();
        
        for (const QJsonValue &value : array) {
            QJsonObject obj = value.toObject();
            Exercise exercise = Exercise::fromJson(obj);
            if (exercise.id() > 0) {
                m_exercises[exercise.id()] = exercise;
            }
        }
    }
    
    // Load workouts
    if (root.contains("workouts")) {
        QJsonArray array = root["workouts"].toArray();
        m_workouts.clear();
        
        for (const QJsonValue &value : array) {
            QJsonObject obj = value.toObject();
            Workout workout = Workout::fromJson(obj);
            if (workout.id() > 0) {
                m_workouts[workout.id()] = workout;
            }
        }
    }
    
    return true;
}

bool DataManager::saveData()
{
    QFile file(m_dataFilePath);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Failed to open data file for writing:" << m_dataFilePath;
        return false;
    }
    
    QJsonObject root;
    QJsonArray bodyCompositionArray;
    
    for (const BodyComposition &composition : m_bodyCompositionData.values()) {
        bodyCompositionArray.append(composition.toJson());
    }
    
    root["bodyComposition"] = bodyCompositionArray;
    
    // Save exercises
    QJsonArray exercisesArray;
    for (const Exercise &exercise : m_exercises.values()) {
        exercisesArray.append(exercise.toJson());
    }
    root["exercises"] = exercisesArray;
    
    // Save workouts
    QJsonArray workoutsArray;
    for (const Workout &workout : m_workouts.values()) {
        workoutsArray.append(workout.toJson());
    }
    root["workouts"] = workoutsArray;
    
    QJsonDocument doc(root);
    qint64 bytesWritten = file.write(doc.toJson());
    
    if (bytesWritten == -1) {
        qWarning() << "Failed to write data file";
        return false;
    }
    
    return true;
}

QString DataManager::getDataFilePath() const
{
    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QString standardPath = appDataPath + "/fitness-tracker/data.json";
    QString actualPath = "/home/paul/.local/share/Fitness Tracker/Fitness Tracker/fitness-tracker/data.json";
    
    printf("Standard path would be: %s\n", standardPath.toLocal8Bit().data());
    printf("Actual data file is at: %s\n", actualPath.toLocal8Bit().data());
    
    // Check if data exists at the actual location
    if (QFile::exists(actualPath)) {
        printf("Using actual data file location\n");
        return actualPath;
    } else {
        printf("Using standard path (no data at actual location)\n");
        return standardPath;
    }
}

void DataManager::ensureDataDirectory() const
{
    QDir dir = QFileInfo(m_dataFilePath).dir();
    if (!dir.exists()) {
        dir.mkpath(".");
    }
}

// Exercise management methods
bool DataManager::saveExercise(const Exercise &exercise)
{
    if (!exercise.isValid()) {
        return false;
    }
    
    Exercise exerciseToSave = exercise;
    if (exerciseToSave.id() == 0) {
        exerciseToSave.setId(getNextExerciseId());
    }
    
    m_exercises[exerciseToSave.id()] = exerciseToSave;
    bool success = saveData();
    if (success) {
        emit dataChanged();
    }
    return success;
}

Exercise DataManager::loadExercise(int id)
{
    return m_exercises.value(id, Exercise());
}

QList<Exercise> DataManager::getAllExercises() const
{
    return m_exercises.values();
}

QList<Exercise> DataManager::getExercisesByCategory(const QString &category) const
{
    QList<Exercise> filtered;
    for (const Exercise &exercise : m_exercises.values()) {
        if (exercise.category() == category && exercise.isActive()) {
            filtered.append(exercise);
        }
    }
    return filtered;
}

bool DataManager::deleteExercise(int id)
{
    if (m_exercises.remove(id) > 0) {
        saveData();
        emit dataChanged();
        return true;
    }
    return false;
}

int DataManager::getNextExerciseId() const
{
    int maxId = 0;
    for (int id : m_exercises.keys()) {
        maxId = qMax(maxId, id);
    }
    return maxId + 1;
}

// Workout management methods
bool DataManager::saveWorkout(const Workout &workout)
{
    if (!workout.isValid()) {
        return false;
    }
    
    Workout workoutToSave = workout;
    if (workoutToSave.id() == 0) {
        workoutToSave.setId(getNextWorkoutId());
    }
    
    m_workouts[workoutToSave.id()] = workoutToSave;
    bool success = saveData();
    if (success) {
        emit dataChanged();
    }
    return success;
}

Workout DataManager::loadWorkout(int id)
{
    return m_workouts.value(id, Workout());
}

QList<Workout> DataManager::getAllWorkouts() const
{
    return m_workouts.values();
}

QList<Workout> DataManager::getWorkoutsByDate(const QDate &date) const
{
    QList<Workout> filtered;
    for (const Workout &workout : m_workouts.values()) {
        if (workout.date() == date) {
            filtered.append(workout);
        }
    }
    return filtered;
}

bool DataManager::deleteWorkout(int id)
{
    if (m_workouts.remove(id) > 0) {
        saveData();
        emit dataChanged();
        return true;
    }
    return false;
}

int DataManager::getNextWorkoutId() const
{
    int maxId = 0;
    for (int id : m_workouts.keys()) {
        maxId = qMax(maxId, id);
    }
    return maxId + 1;
} 