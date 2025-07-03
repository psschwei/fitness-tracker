#ifndef WORKOUTBUILDER_H
#define WORKOUTBUILDER_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QComboBox>
#include <QGroupBox>
#include <QMessageBox>
#include <QDate>
#include <QList>
#include <QDoubleValidator>
#include <QIntValidator>
#include "models/datamanager.h"
#include "models/exercise.h"
#include "models/workout.h"
#include "models/setdata.h"

class WorkoutBuilder : public QWidget
{
    Q_OBJECT

public:
    explicit WorkoutBuilder(DataManager *dataManager, QWidget *parent = nullptr);
    void setDate(const QDate &date);

signals:
    void workoutCreated();

public slots:
    void updateExerciseComboBox();

private slots:
    void saveWorkout();
    void clearForm();

private:
    void setupUI();
    void addExerciseRow();
    bool validateForm();
    void showError(const QString &message);
    void showSuccess(const QString &message);
    
    DataManager *m_dataManager;
    QDate m_currentDate;
    
    // UI elements
    QTextEdit *m_workoutNotesEdit;
    QPushButton *m_saveButton;
    QPushButton *m_clearButton;
    QWidget *m_exercisesContainer;
    
    // Exercise rows
    struct ExerciseRow {
        QComboBox *exerciseCombo;
        QLineEdit *weightEdit;
        QLineEdit *repsEdit;
        QLineEdit *setsEdit;
        QTextEdit *notesEdit;
    };
    
    QList<ExerciseRow> m_exerciseRows;
    QList<Exercise> m_availableExercises;
};

#endif // WORKOUTBUILDER_H 