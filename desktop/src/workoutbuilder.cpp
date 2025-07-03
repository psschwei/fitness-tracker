#include "workoutbuilder.h"
#include <QDebug>
#include <QMessageBox>

WorkoutBuilder::WorkoutBuilder(DataManager *dataManager, QWidget *parent)
    : QWidget(parent)
    , m_dataManager(dataManager)
    , m_currentDate(QDate::currentDate())
{
    setupUI();
    updateExerciseComboBox();
}

void WorkoutBuilder::setDate(const QDate &date)
{
    m_currentDate = date;
}

void WorkoutBuilder::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(40, 20, 40, 20);
    
    // Workout notes section
    QGroupBox *notesGroup = new QGroupBox("Workout Notes (Optional)");
    QVBoxLayout *notesLayout = new QVBoxLayout(notesGroup);
    
    m_workoutNotesEdit = new QTextEdit();
    m_workoutNotesEdit->setMaximumHeight(50);
    m_workoutNotesEdit->setPlaceholderText("Add notes about this workout...");
    notesLayout->addWidget(m_workoutNotesEdit);
    
    mainLayout->addWidget(notesGroup);
    
    // Exercises section
    QGroupBox *exercisesGroup = new QGroupBox("Exercises");
    QVBoxLayout *exercisesLayout = new QVBoxLayout(exercisesGroup);
    
    // Create exercises container first
    m_exercisesContainer = new QWidget();
    QVBoxLayout *containerLayout = new QVBoxLayout(m_exercisesContainer);
    containerLayout->setAlignment(Qt::AlignTop);
    
    // Create 8 exercise rows
    for (int i = 0; i < 8; ++i) {
        addExerciseRow();
    }
    
    exercisesLayout->addWidget(m_exercisesContainer);
    
    mainLayout->addWidget(exercisesGroup);
    
    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    
    m_saveButton = new QPushButton("Save Workout");
    m_saveButton->setStyleSheet("QPushButton { padding: 10px; font-weight: bold; background-color: #4CAF50; color: white; border: none; border-radius: 4px; }");
    connect(m_saveButton, &QPushButton::clicked, this, &WorkoutBuilder::saveWorkout);
    
    m_clearButton = new QPushButton("Clear Form");
    m_clearButton->setStyleSheet("QPushButton { padding: 10px; font-weight: bold; background-color: #f44336; color: white; border: none; border-radius: 4px; }");
    connect(m_clearButton, &QPushButton::clicked, this, &WorkoutBuilder::clearForm);
    
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_clearButton);
    buttonLayout->addWidget(m_saveButton);
    
    mainLayout->addLayout(buttonLayout);
    
    setLayout(mainLayout);
}

void WorkoutBuilder::updateExerciseComboBox()
{
    m_availableExercises = m_dataManager->getAllExercises();
    
    // Update all existing combo boxes
    for (ExerciseRow &row : m_exerciseRows) {
        row.exerciseCombo->clear();
        row.exerciseCombo->addItem("Select an exercise", -1);
        
        for (const Exercise &exercise : m_availableExercises) {
            if (exercise.isActive()) {
                row.exerciseCombo->addItem(exercise.name(), exercise.id());
            }
        }
    }
}

void WorkoutBuilder::addExerciseRow()
{
    ExerciseRow row;
    
    // Exercise selection
    row.exerciseCombo = new QComboBox();
    row.exerciseCombo->addItem("Select an exercise", -1);
    for (const Exercise &exercise : m_availableExercises) {
        if (exercise.isActive()) {
            row.exerciseCombo->addItem(exercise.name(), exercise.id());
        }
    }
    
    // Weight input (text input instead of spinner)
    row.weightEdit = new QLineEdit();
    row.weightEdit->setPlaceholderText("0.0");
    row.weightEdit->setValidator(new QDoubleValidator(0.0, 999.9, 1, this));
    row.weightEdit->setMaximumWidth(60);
    row.weightEdit->setMinimumWidth(60);
    
    // Reps input (text input instead of spinner)
    row.repsEdit = new QLineEdit();
    row.repsEdit->setPlaceholderText("0");
    row.repsEdit->setValidator(new QIntValidator(0, 999, this));
    row.repsEdit->setMaximumWidth(50);
    row.repsEdit->setMinimumWidth(50);
    
    // Sets input (text input instead of spinner)
    row.setsEdit = new QLineEdit();
    row.setsEdit->setPlaceholderText("1");
    row.setsEdit->setValidator(new QIntValidator(1, 99, this));
    row.setsEdit->setText("1");
    row.setsEdit->setMaximumWidth(50);
    row.setsEdit->setMinimumWidth(50);
    
    // Notes input
    row.notesEdit = new QTextEdit();
    row.notesEdit->setMaximumHeight(40);
    row.notesEdit->setPlaceholderText("Notes...");
    
    // Create layout for this row (no group box title)
    QWidget *exerciseWidget = new QWidget();
    QGridLayout *exerciseLayout = new QGridLayout(exerciseWidget);
    exerciseLayout->setContentsMargins(10, 5, 10, 5);
    
    exerciseLayout->addWidget(new QLabel("Exercise:"), 0, 0);
    exerciseLayout->addWidget(row.exerciseCombo, 0, 1);
    exerciseLayout->addWidget(new QLabel("Weight:"), 0, 2);
    exerciseLayout->addWidget(row.weightEdit, 0, 3);
    exerciseLayout->addWidget(new QLabel("Reps:"), 0, 4);
    exerciseLayout->addWidget(row.repsEdit, 0, 5);
    exerciseLayout->addWidget(new QLabel("Sets:"), 0, 6);
    exerciseLayout->addWidget(row.setsEdit, 0, 7);
    
    exerciseLayout->addWidget(new QLabel("Notes:"), 1, 0);
    exerciseLayout->addWidget(row.notesEdit, 1, 1, 1, 7);
    
    // Add to container
    QVBoxLayout *containerLayout = qobject_cast<QVBoxLayout*>(m_exercisesContainer->layout());
    containerLayout->addWidget(exerciseWidget);
    
    m_exerciseRows.append(row);
}

void WorkoutBuilder::saveWorkout()
{
    if (!validateForm()) {
        return;
    }
    
    // Create workout
    Workout workout(0, m_currentDate, m_workoutNotesEdit->toPlainText().trimmed());
    
    // Add exercises (only those with selected exercise names)
    for (const ExerciseRow &row : m_exerciseRows) {
        int exerciseId = row.exerciseCombo->currentData().toInt();
        if (exerciseId > 0) {
            // Find exercise name
            QString exerciseName;
            for (const Exercise &exercise : m_availableExercises) {
                if (exercise.id() == exerciseId) {
                    exerciseName = exercise.name();
                    break;
                }
            }
            
            // Get values from text inputs
            double weight = row.weightEdit->text().toDouble();
            int reps = row.repsEdit->text().toInt();
            int sets = row.setsEdit->text().toInt();
            
            // Create sets data
            QList<SetData> setsData;
            SetData set(weight, reps, sets);
            setsData.append(set);
            
            // Create workout exercise
            WorkoutExercise workoutExercise(0, 0, exerciseId, exerciseName, setsData, row.notesEdit->toPlainText().trimmed());
            workout.addExercise(workoutExercise);
        }
    }
    
    // Save workout
    if (m_dataManager->saveWorkout(workout)) {
        showSuccess("Workout saved successfully!");
        emit workoutCreated();
        clearForm();
    } else {
        showError("Failed to save workout. Please check your data.");
    }
}

void WorkoutBuilder::clearForm()
{
    m_workoutNotesEdit->clear();
    
    // Clear all exercise rows
    for (ExerciseRow &row : m_exerciseRows) {
        row.exerciseCombo->setCurrentIndex(0);
        row.weightEdit->clear();
        row.repsEdit->clear();
        row.setsEdit->setText("1");
        row.notesEdit->clear();
    }
}

bool WorkoutBuilder::validateForm()
{
    // Check if at least one exercise is selected
    bool hasValidExercise = false;
    for (const ExerciseRow &row : m_exerciseRows) {
        if (row.exerciseCombo->currentData().toInt() > 0) {
            hasValidExercise = true;
            break;
        }
    }
    
    if (!hasValidExercise) {
        showError("Please select at least one exercise for the workout.");
        return false;
    }
    
    // Validate each exercise row that has an exercise selected
    for (int i = 0; i < m_exerciseRows.size(); ++i) {
        const ExerciseRow &row = m_exerciseRows[i];
        int exerciseId = row.exerciseCombo->currentData().toInt();
        
        if (exerciseId > 0) {
            // Exercise is selected, validate the data
            if (row.weightEdit->text().isEmpty()) {
                showError(QString("Exercise %1: Please enter weight.").arg(i + 1));
                return false;
            }
            
            double weight = row.weightEdit->text().toDouble();
            if (weight < 0) {
                showError(QString("Exercise %1: Weight cannot be negative.").arg(i + 1));
                return false;
            }
            
            if (row.repsEdit->text().isEmpty()) {
                showError(QString("Exercise %1: Please enter reps.").arg(i + 1));
                return false;
            }
            
            int reps = row.repsEdit->text().toInt();
            if (reps <= 0) {
                showError(QString("Exercise %1: Reps must be greater than 0.").arg(i + 1));
                return false;
            }
            
            if (row.setsEdit->text().isEmpty()) {
                showError(QString("Exercise %1: Please enter sets.").arg(i + 1));
                return false;
            }
            
            int sets = row.setsEdit->text().toInt();
            if (sets <= 0) {
                showError(QString("Exercise %1: Sets must be greater than 0.").arg(i + 1));
                return false;
            }
        }
    }
    
    return true;
}

void WorkoutBuilder::showError(const QString &message)
{
    QMessageBox::warning(this, "Validation Error", message);
}

void WorkoutBuilder::showSuccess(const QString &message)
{
    QMessageBox::information(this, "Success", message);
} 