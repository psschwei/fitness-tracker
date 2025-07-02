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
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QScrollArea>
#include <QGroupBox>
#include <QMessageBox>
#include <QDate>
#include <QList>
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
    void addExercise();
    void removeExercise(int index);
    void saveWorkout();
    void clearForm();

private:
    void setupUI();
    void addExerciseRow();
    void removeExerciseRow(int index);
    bool validateForm();
    void showError(const QString &message);
    void showSuccess(const QString &message);
    
    DataManager *m_dataManager;
    QDate m_currentDate;
    
    // UI elements
    QTextEdit *m_workoutNotesEdit;
    QPushButton *m_addExerciseButton;
    QPushButton *m_saveButton;
    QPushButton *m_clearButton;
    QScrollArea *m_exercisesScrollArea;
    QWidget *m_exercisesContainer;
    
    // Exercise rows
    struct ExerciseRow {
        QComboBox *exerciseCombo;
        QDoubleSpinBox *weightSpin;
        QSpinBox *repsSpin;
        QSpinBox *setsSpin;
        QTextEdit *notesEdit;
        QPushButton *removeButton;
    };
    
    QList<ExerciseRow> m_exerciseRows;
    QList<Exercise> m_availableExercises;
};

#endif // WORKOUTBUILDER_H 