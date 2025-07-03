#ifndef WORKOUTVIEW_H
#define WORKOUTVIEW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QGroupBox>
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
    void editRequested();
    void deleteRequested();

private slots:
    void onEditClicked();
    void onDeleteClicked();

private:
    void setupUI();
    void displayWorkouts();
    void clearWorkouts();
    
    DataManager *m_dataManager;
    QDate m_currentDate;
    
    // UI elements
    QLabel *m_noWorkoutsLabel;
    QWidget *m_workoutsContainer;
    QPushButton *m_editButton;
    QPushButton *m_deleteButton;
    QList<Workout> m_currentWorkouts;
};

#endif // WORKOUTVIEW_H 