#ifndef JOURNALCONTENTAREA_H
#define JOURNALCONTENTAREA_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QPushButton>
#include <QTabWidget>
#include "datemanager.h"
#include "bodycompositionform.h"
#include "bodycompositionview.h"
#include "workoutbuilder.h"
#include "workoutview.h"
#include "exerciselibrary.h"
#include "models/datamanager.h"

class JournalContentArea : public QWidget
{
    Q_OBJECT

public:
    explicit JournalContentArea(DateManager *dateManager, ::DataManager *dataManager, QWidget *parent = nullptr);

private slots:
    void onDateChanged(const QDate &newDate);
    void onDataSaved(const BodyComposition &data);
    void onCancelled();
    void onEditRequested();
    void onDeleteRequested();
    void onAddNewClicked();
    void onWorkoutCreated();
    void onWorkoutEditRequested();
    void onWorkoutDeleteRequested();
    void onWorkoutCancelled();
    void onExerciseAdded();
    void onExerciseUpdated();
    void onExerciseDeleted();
    void setupKeyboardShortcuts();
    void switchToBodyComposition();
    void switchToWorkouts();
    void switchToExerciseLibrary();
    void onAddWorkoutClicked();
    void onEditShortcut();
    void onDeleteShortcut();
    void onAddShortcut();
    void onTodayShortcut();
    void onNextDayShortcut();
    void onPrevDayShortcut();

private:
    void setupUI();
    void updateContent();
    void showEmptyState();
    void showBodyCompositionForm();
    void showBodyCompositionView(const BodyComposition &data);
    void loadDataForCurrentDate();

    DateManager *m_dateManager;
    ::DataManager *m_dataManager;
    QLabel *m_contentLabel;
    QStackedWidget *m_stackedWidget;
    BodyCompositionForm *m_bodyCompositionForm;
    BodyCompositionView *m_bodyCompositionView;
    QPushButton *m_addNewButton;
    
    // Exercise tracking components
    QTabWidget *m_tabWidget;
    QStackedWidget *m_workoutStackedWidget;
    QWidget *m_workoutEmptyStateWidget;
    WorkoutBuilder *m_workoutBuilder;
    WorkoutView *m_workoutView;
    ExerciseLibrary *m_exerciseLibrary;
};

#endif // JOURNALCONTENTAREA_H 