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
    mainLayout->setContentsMargins(40, 20, 40, 20);
    
    // No workouts label
    m_noWorkoutsLabel = new QLabel("No workouts for this date");
    m_noWorkoutsLabel->setAlignment(Qt::AlignCenter);
    m_noWorkoutsLabel->setStyleSheet("QLabel { color: #666; font-size: 14px; padding: 20px; }");
    mainLayout->addWidget(m_noWorkoutsLabel);
    
    // Workouts container
    m_workoutsContainer = new QWidget();
    QVBoxLayout *containerLayout = new QVBoxLayout(m_workoutsContainer);
    containerLayout->setAlignment(Qt::AlignTop);
    
    mainLayout->addWidget(m_workoutsContainer);
    
    // Create buttons
    m_editButton = new QPushButton("Edit", this);
    m_editButton->setStyleSheet("QPushButton { background-color: #007bff; color: white; border: none; padding: 8px 16px; border-radius: 4px; }");
    connect(m_editButton, &QPushButton::clicked, this, &WorkoutView::onEditClicked);
    
    m_deleteButton = new QPushButton("Delete", this);
    m_deleteButton->setStyleSheet("QPushButton { background-color: #dc3545; color: white; border: none; padding: 8px 16px; border-radius: 4px; }");
    connect(m_deleteButton, &QPushButton::clicked, this, &WorkoutView::onDeleteClicked);
    
    // Create button layout
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_deleteButton);
    buttonLayout->addWidget(m_editButton);
    
    mainLayout->addStretch();
    mainLayout->addLayout(buttonLayout);
    
    // Initial data load
    refreshData();
}

void WorkoutView::displayWorkouts()
{
    clearWorkouts();
    
    if (m_currentWorkouts.isEmpty()) {
        m_noWorkoutsLabel->show();
        m_workoutsContainer->hide();
        m_editButton->hide();
        m_deleteButton->hide();
        return;
    }
    
    m_noWorkoutsLabel->hide();
    m_workoutsContainer->show();
    m_editButton->show();
    m_deleteButton->show();
    
    QVBoxLayout *containerLayout = qobject_cast<QVBoxLayout*>(m_workoutsContainer->layout());
    
    // Defensive programming: try to display workouts, but catch any crashes
    try {
    
    for (const Workout &workout : m_currentWorkouts) {
        // Create workout group
        QGroupBox *workoutGroup = new QGroupBox("Workout");
        workoutGroup->setStyleSheet("QGroupBox { font-weight: bold; margin-top: 10px; }");
        
        QVBoxLayout *workoutLayout = new QVBoxLayout(workoutGroup);
        
        // Workout info
        QHBoxLayout *infoLayout = new QHBoxLayout();
        
        QLabel *statusLabel = new QLabel(QString("Status: %1").arg(workout.statusString()));
        statusLabel->setStyleSheet("QLabel { font-weight: normal; }");
        
        infoLayout->addWidget(statusLabel);
        infoLayout->addStretch();
        
        workoutLayout->addLayout(infoLayout);
        
        // Workout notes
        if (!workout.notes().isEmpty()) {
            QLabel *notesLabel = new QLabel(QString("Notes: %1").arg(workout.notes()));
            notesLabel->setWordWrap(true);
            notesLabel->setStyleSheet("QLabel { font-weight: normal; background-color: #f5f5f5; padding: 8px; border-radius: 4px; }");
            workoutLayout->addWidget(notesLabel);
        }
        
        // Exercises - defensive copy to prevent segfault
        const QList<WorkoutExercise> exercises = workout.exercises();
        if (!exercises.isEmpty()) {
            QLabel *exercisesLabel = new QLabel("Exercises:");
            exercisesLabel->setStyleSheet("QLabel { font-weight: bold; margin-top: 10px; }");
            workoutLayout->addWidget(exercisesLabel);
            
            for (int i = 0; i < exercises.size(); ++i) {
                const WorkoutExercise &exercise = exercises[i];
                
                QGroupBox *exerciseGroup = new QGroupBox(QString("%1. %2").arg(i + 1).arg(exercise.exerciseName()));
                exerciseGroup->setStyleSheet("QGroupBox { font-weight: normal; margin: 5px; }");
                
                QVBoxLayout *exerciseLayout = new QVBoxLayout(exerciseGroup);
                
                // Exercise details - defensive copy to prevent segfault
                const QList<SetData> sets = exercise.setsData();
                if (!sets.isEmpty()) {
                    for (int j = 0; j < sets.size(); ++j) {
                        const SetData &set = sets.at(j);
                        QString setText = QString("Set %1: %2 kg × %3 reps × %4 sets")
                                            .arg(j + 1)
                                            .arg(set.weight())
                                            .arg(set.reps())
                                            .arg(set.sets());
                        
                        QLabel *setLabel = new QLabel(setText);
                        setLabel->setStyleSheet("QLabel { margin-left: 10px; }");
                        exerciseLayout->addWidget(setLabel);
                    }
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
    } catch (...) {
        // If there's any crash while displaying workouts, show error message
        QLabel *errorLabel = new QLabel("Error displaying workout data");
        errorLabel->setStyleSheet("QLabel { color: red; font-weight: bold; padding: 10px; }");
        containerLayout->addWidget(errorLabel);
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

void WorkoutView::onEditClicked()
{
    emit editRequested();
}

void WorkoutView::onDeleteClicked()
{
    if (m_currentWorkouts.isEmpty()) {
        return;
    }
    
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, 
        "Delete Workout", 
        "Are you sure you want to delete this workout? This action cannot be undone.",
        QMessageBox::Yes | QMessageBox::No
    );
    
    if (reply == QMessageBox::Yes) {
        // Delete all workouts for this date
        for (const Workout &workout : m_currentWorkouts) {
            m_dataManager->deleteWorkout(workout.id());
        }
        emit deleteRequested();
    }
} 