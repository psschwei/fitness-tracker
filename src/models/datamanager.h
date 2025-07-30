#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QObject>
#include <QDate>
#include <QMap>
#include <QList>
#include "bodycomposition.h"
#include "exercise.h"
#include "workout.h"

class DataManager : public QObject
{
    Q_OBJECT

public:
    explicit DataManager(QObject *parent = nullptr);
    
    // Body composition data management
    bool saveBodyComposition(const BodyComposition &data);
    BodyComposition loadBodyComposition(const QDate &date);
    bool hasBodyComposition(const QDate &date) const;
    void deleteBodyComposition(const QDate &date);
    QList<QDate> getBodyCompositionDates() const;
    
    // Analytics data aggregation methods
    QList<BodyComposition> getBodyCompositionRange(const QDate &start, const QDate &end) const;
    QList<BodyComposition> getAllBodyCompositionSorted() const;
    QPair<double, double> getWeightRange() const;
    QPair<QDate, QDate> getDataDateRange() const;
    
    // Exercise library management
    bool saveExercise(const Exercise &exercise);
    Exercise loadExercise(int id);
    QList<Exercise> getAllExercises() const;
    QList<Exercise> getExercisesByCategory(const QString &category) const;
    bool deleteExercise(int id);
    int getNextExerciseId() const;
    
    // Workout management
    bool saveWorkout(const Workout &workout);
    Workout loadWorkout(int id);
    QList<Workout> getAllWorkouts() const;
    QList<Workout> getWorkoutsByDate(const QDate &date) const;
    bool deleteWorkout(int id);
    int getNextWorkoutId() const;
    
    // Data persistence
    bool loadData();
    bool saveData();

signals:
    void dataChanged();

private:
    QString getDataFilePath() const;
    void ensureDataDirectory() const;
    
    QMap<QDate, BodyComposition> m_bodyCompositionData;
    QMap<int, Exercise> m_exercises;
    QMap<int, Workout> m_workouts;
    QString m_dataFilePath;
};

#endif // DATAMANAGER_H 