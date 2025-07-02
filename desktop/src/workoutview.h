#ifndef WORKOUTVIEW_H
#define WORKOUTVIEW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QGroupBox>
#include <QTextEdit>
#include <QDate>
#include <QList>
#include "models/datamanager.h"
#include "models/workout.h"

class WorkoutView : public QWidget
{
    Q_OBJECT

public:
    explicit WorkoutView(DataManager *dataManager, QWidget *parent = nullptr);
    void setDate(const QDate &date);
    void refreshData();

signals:
    void workoutDeleted();

private slots:
    void deleteWorkout(int workoutId);

private:
    void setupUI();
    void displayWorkouts();
    void clearWorkouts();
    
    DataManager *m_dataManager;
    QDate m_currentDate;
    
    // UI elements
    QLabel *m_noWorkoutsLabel;
    QScrollArea *m_workoutsScrollArea;
    QWidget *m_workoutsContainer;
    QList<Workout> m_currentWorkouts;
};

#endif // WORKOUTVIEW_H 