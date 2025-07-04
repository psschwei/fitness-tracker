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
    void loadWorkoutData(const QList<Workout> &workouts);
    void clearForm();
    void setEditingWorkoutId(int workoutId);

signals:
    void workoutCreated();
    void cancelled();

public slots:
    void updateExerciseComboBox();

private slots:
    void saveWorkout();
    void onCancelClicked();

private:
    void setupUI();
    void addExerciseRow();
    bool validateForm();
    void showError(const QString &message);
    void showSuccess(const QString &message);
    bool eventFilter(QObject *obj, QEvent *event) override;
    
    DataManager *m_dataManager;
    QDate m_currentDate;
    int m_editingWorkoutId;
    
    // UI elements
    QTextEdit *m_workoutNotesEdit;
    QPushButton *m_saveButton;
    QPushButton *m_cancelButton;
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