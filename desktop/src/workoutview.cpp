#include "workoutview.h"
#include <QMessageBox>
#include <QDebug>

WorkoutView::WorkoutView(DataManager *dataManager, QWidget *parent)
    : QWidget(parent)
    , m_dataManager(dataManager)
    , m_currentDate(QDate::currentDate())
{
    setupUI();
}

void WorkoutView::setDate(const QDate &date)
{
    m_currentDate = date;
    refreshData();
}

void WorkoutView::refreshData()
{
    m_currentWorkouts = m_dataManager->getWorkoutsByDate(m_currentDate);
    displayWorkouts();
}

void WorkoutView::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // No workouts label
    m_noWorkoutsLabel = new QLabel("No workouts for this date");
    m_noWorkoutsLabel->setAlignment(Qt::AlignCenter);
    m_noWorkoutsLabel->setStyleSheet("QLabel { color: #666; font-size: 14px; padding: 20px; }");
    mainLayout->addWidget(m_noWorkoutsLabel);
    
    // Workouts scroll area
    m_workoutsScrollArea = new QScrollArea();
    m_workoutsContainer = new QWidget();
    QVBoxLayout *containerLayout = new QVBoxLayout(m_workoutsContainer);
    containerLayout->setAlignment(Qt::AlignTop);
    
    m_workoutsScrollArea->setWidget(m_workoutsContainer);
    m_workoutsScrollArea->setWidgetResizable(true);
    m_workoutsScrollArea->setMaximumHeight(500);
    
    mainLayout->addWidget(m_workoutsScrollArea);
    
    // Initial data load
    refreshData();
}

void WorkoutView::displayWorkouts()
{
    clearWorkouts();
    
    if (m_currentWorkouts.isEmpty()) {
        m_noWorkoutsLabel->show();
        m_workoutsScrollArea->hide();
        return;
    }
    
    m_noWorkoutsLabel->hide();
    m_workoutsScrollArea->show();
    
    QVBoxLayout *containerLayout = qobject_cast<QVBoxLayout*>(m_workoutsContainer->layout());
    
    for (const Workout &workout : m_currentWorkouts) {
        // Create workout group
        QGroupBox *workoutGroup = new QGroupBox(QString("Workout #%1").arg(workout.id()));
        workoutGroup->setStyleSheet("QGroupBox { font-weight: bold; margin-top: 10px; }");
        
        QVBoxLayout *workoutLayout = new QVBoxLayout(workoutGroup);
        
        // Workout info
        QHBoxLayout *infoLayout = new QHBoxLayout();
        
        QLabel *statusLabel = new QLabel(QString("Status: %1").arg(workout.statusString()));
        statusLabel->setStyleSheet("QLabel { font-weight: normal; }");
        
        QPushButton *deleteButton = new QPushButton("Delete");
        deleteButton->setStyleSheet("QPushButton { background-color: #f44336; color: white; border: none; border-radius: 4px; padding: 4px 8px; }");
        connect(deleteButton, &QPushButton::clicked, [this, workout]() {
            deleteWorkout(workout.id());
        });
        
        infoLayout->addWidget(statusLabel);
        infoLayout->addStretch();
        infoLayout->addWidget(deleteButton);
        
        workoutLayout->addLayout(infoLayout);
        
        // Workout notes
        if (!workout.notes().isEmpty()) {
            QLabel *notesLabel = new QLabel(QString("Notes: %1").arg(workout.notes()));
            notesLabel->setWordWrap(true);
            notesLabel->setStyleSheet("QLabel { font-weight: normal; background-color: #f5f5f5; padding: 8px; border-radius: 4px; }");
            workoutLayout->addWidget(notesLabel);
        }
        
        // Exercises
        if (!workout.exercises().isEmpty()) {
            QLabel *exercisesLabel = new QLabel("Exercises:");
            exercisesLabel->setStyleSheet("QLabel { font-weight: bold; margin-top: 10px; }");
            workoutLayout->addWidget(exercisesLabel);
            
            for (int i = 0; i < workout.exercises().size(); ++i) {
                const WorkoutExercise &exercise = workout.exercises()[i];
                
                QGroupBox *exerciseGroup = new QGroupBox(QString("%1. %2").arg(i + 1).arg(exercise.exerciseName()));
                exerciseGroup->setStyleSheet("QGroupBox { font-weight: normal; margin: 5px; }");
                
                QVBoxLayout *exerciseLayout = new QVBoxLayout(exerciseGroup);
                
                // Exercise details
                for (int j = 0; j < exercise.setsData().size(); ++j) {
                    const SetData &set = exercise.setsData()[j];
                    QString setText = QString("Set %1: %2 kg × %3 reps × %4 sets")
                                        .arg(j + 1)
                                        .arg(set.weight())
                                        .arg(set.reps())
                                        .arg(set.sets());
                    
                    QLabel *setLabel = new QLabel(setText);
                    setLabel->setStyleSheet("QLabel { margin-left: 10px; }");
                    exerciseLayout->addWidget(setLabel);
                }
                
                // Exercise notes
                if (!exercise.notes().isEmpty()) {
                    QLabel *exerciseNotesLabel = new QLabel(QString("Notes: %1").arg(exercise.notes()));
                    exerciseNotesLabel->setWordWrap(true);
                    exerciseNotesLabel->setStyleSheet("QLabel { margin-left: 10px; font-style: italic; color: #666; }");
                    exerciseLayout->addWidget(exerciseNotesLabel);
                }
                
                workoutLayout->addWidget(exerciseGroup);
            }
        }
        
        containerLayout->addWidget(workoutGroup);
    }
}

void WorkoutView::clearWorkouts()
{
    QVBoxLayout *containerLayout = qobject_cast<QVBoxLayout*>(m_workoutsContainer->layout());
    if (containerLayout) {
        QLayoutItem *item;
        while ((item = containerLayout->takeAt(0)) != nullptr) {
            if (item->widget()) {
                item->widget()->deleteLater();
            }
            delete item;
        }
    }
}

void WorkoutView::deleteWorkout(int workoutId)
{
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, 
        "Delete Workout", 
        "Are you sure you want to delete this workout? This action cannot be undone.",
        QMessageBox::Yes | QMessageBox::No
    );
    
    if (reply == QMessageBox::Yes) {
        if (m_dataManager->deleteWorkout(workoutId)) {
            refreshData();
            emit workoutDeleted();
        } else {
            QMessageBox::warning(this, "Error", "Failed to delete workout.");
        }
    }
} 