#include "workoutbuilder.h"
#include <QDebug>

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
    
    // Workout notes section
    QGroupBox *notesGroup = new QGroupBox("Workout Notes (Optional)");
    QVBoxLayout *notesLayout = new QVBoxLayout(notesGroup);
    
    m_workoutNotesEdit = new QTextEdit();
    m_workoutNotesEdit->setMaximumHeight(80);
    m_workoutNotesEdit->setPlaceholderText("Add notes about this workout...");
    notesLayout->addWidget(m_workoutNotesEdit);
    
    mainLayout->addWidget(notesGroup);
    
    // Exercises section
    QGroupBox *exercisesGroup = new QGroupBox("Exercises");
    QVBoxLayout *exercisesLayout = new QVBoxLayout(exercisesGroup);
    
    // Exercise rows container
    m_exercisesScrollArea = new QScrollArea();
    m_exercisesContainer = new QWidget();
    QVBoxLayout *containerLayout = new QVBoxLayout(m_exercisesContainer);
    containerLayout->setAlignment(Qt::AlignTop);
    
    m_exercisesScrollArea->setWidget(m_exercisesContainer);
    m_exercisesScrollArea->setWidgetResizable(true);
    m_exercisesScrollArea->setMaximumHeight(400);
    
    exercisesLayout->addWidget(m_exercisesScrollArea);
    
    // Add exercise button
    m_addExerciseButton = new QPushButton("+ Add Exercise");
    m_addExerciseButton->setStyleSheet("QPushButton { padding: 8px; font-weight: bold; }");
    connect(m_addExerciseButton, &QPushButton::clicked, this, &WorkoutBuilder::addExercise);
    exercisesLayout->addWidget(m_addExerciseButton);
    
    mainLayout->addWidget(exercisesGroup);
    
    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    
    m_saveButton = new QPushButton("Save Workout");
    m_saveButton->setStyleSheet("QPushButton { padding: 10px; font-weight: bold; background-color: #4CAF50; color: white; border: none; border-radius: 4px; }");
    connect(m_saveButton, &QPushButton::clicked, this, &WorkoutBuilder::saveWorkout);
    
    m_clearButton = new QPushButton("Clear Form");
    m_clearButton->setStyleSheet("QPushButton { padding: 10px; font-weight: bold; background-color: #f44336; color: white; border: none; border-radius: 4px; }");
    connect(m_clearButton, &QPushButton::clicked, this, &WorkoutBuilder::clearForm);
    
    buttonLayout->addWidget(m_saveButton);
    buttonLayout->addWidget(m_clearButton);
    
    mainLayout->addLayout(buttonLayout);
    
    // Add initial exercise row
    addExerciseRow();
}

void WorkoutBuilder::updateExerciseComboBox()
{
    m_availableExercises = m_dataManager->getAllExercises();
    
    // Update all existing combo boxes
    for (ExerciseRow &row : m_exerciseRows) {
        row.exerciseCombo->clear();
        row.exerciseCombo->addItem("Select Exercise", -1);
        
        for (const Exercise &exercise : m_availableExercises) {
            if (exercise.isActive()) {
                row.exerciseCombo->addItem(exercise.name(), exercise.id());
            }
        }
    }
}

void WorkoutBuilder::addExercise()
{
    addExerciseRow();
}

void WorkoutBuilder::removeExercise(int index)
{
    removeExerciseRow(index);
}

void WorkoutBuilder::addExerciseRow()
{
    ExerciseRow row;
    
    // Exercise selection
    row.exerciseCombo = new QComboBox();
    row.exerciseCombo->addItem("Select Exercise", -1);
    for (const Exercise &exercise : m_availableExercises) {
        if (exercise.isActive()) {
            row.exerciseCombo->addItem(exercise.name(), exercise.id());
        }
    }
    
    // Weight input
    row.weightSpin = new QDoubleSpinBox();
    row.weightSpin->setRange(0.0, 999.9);
    row.weightSpin->setSuffix(" kg");
    row.weightSpin->setDecimals(1);
    row.weightSpin->setSingleStep(0.5);
    
    // Reps input
    row.repsSpin = new QSpinBox();
    row.repsSpin->setRange(1, 999);
    row.repsSpin->setSuffix(" reps");
    
    // Sets input
    row.setsSpin = new QSpinBox();
    row.setsSpin->setRange(1, 99);
    row.setsSpin->setSuffix(" sets");
    row.setsSpin->setValue(1);
    
    // Notes input
    row.notesEdit = new QTextEdit();
    row.notesEdit->setMaximumHeight(60);
    row.notesEdit->setPlaceholderText("Exercise notes...");
    
    // Remove button
    row.removeButton = new QPushButton("Remove");
    row.removeButton->setStyleSheet("QPushButton { background-color: #f44336; color: white; border: none; border-radius: 4px; padding: 4px 8px; }");
    
    // Connect remove button
    int rowIndex = m_exerciseRows.size();
    connect(row.removeButton, &QPushButton::clicked, [this, rowIndex]() {
        removeExercise(rowIndex);
    });
    
    // Create layout for this row
    QGroupBox *exerciseGroup = new QGroupBox(QString("Exercise %1").arg(rowIndex + 1));
    QGridLayout *exerciseLayout = new QGridLayout(exerciseGroup);
    
    exerciseLayout->addWidget(new QLabel("Exercise:"), 0, 0);
    exerciseLayout->addWidget(row.exerciseCombo, 0, 1);
    exerciseLayout->addWidget(new QLabel("Weight:"), 0, 2);
    exerciseLayout->addWidget(row.weightSpin, 0, 3);
    exerciseLayout->addWidget(new QLabel("Reps:"), 0, 4);
    exerciseLayout->addWidget(row.repsSpin, 0, 5);
    exerciseLayout->addWidget(new QLabel("Sets:"), 0, 6);
    exerciseLayout->addWidget(row.setsSpin, 0, 7);
    exerciseLayout->addWidget(row.removeButton, 0, 8);
    
    exerciseLayout->addWidget(new QLabel("Notes:"), 1, 0);
    exerciseLayout->addWidget(row.notesEdit, 1, 1, 1, 8);
    
    // Add to container
    QVBoxLayout *containerLayout = qobject_cast<QVBoxLayout*>(m_exercisesContainer->layout());
    containerLayout->addWidget(exerciseGroup);
    
    m_exerciseRows.append(row);
}

void WorkoutBuilder::removeExerciseRow(int index)
{
    if (index >= 0 && index < m_exerciseRows.size()) {
        // Remove the widget from the container
        QVBoxLayout *containerLayout = qobject_cast<QVBoxLayout*>(m_exercisesContainer->layout());
        if (containerLayout && index < containerLayout->count()) {
            QLayoutItem *item = containerLayout->takeAt(index);
            if (item->widget()) {
                item->widget()->deleteLater();
            }
            delete item;
        }
        
        // Remove from our list
        m_exerciseRows.removeAt(index);
        
        // Update group box titles
        for (int i = 0; i < m_exerciseRows.size(); ++i) {
            QVBoxLayout *containerLayout = qobject_cast<QVBoxLayout*>(m_exercisesContainer->layout());
            if (containerLayout && i < containerLayout->count()) {
                QLayoutItem *item = containerLayout->itemAt(i);
                if (item && item->widget()) {
                    QGroupBox *groupBox = qobject_cast<QGroupBox*>(item->widget());
                    if (groupBox) {
                        groupBox->setTitle(QString("Exercise %1").arg(i + 1));
                    }
                }
            }
        }
    }
}

void WorkoutBuilder::saveWorkout()
{
    if (!validateForm()) {
        return;
    }
    
    // Create workout
    Workout workout(0, m_currentDate, m_workoutNotesEdit->toPlainText().trimmed());
    
    // Add exercises
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
            
            // Create sets data
            QList<SetData> setsData;
            SetData set(row.weightSpin->value(), row.repsSpin->value(), row.setsSpin->value());
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
    
    // Remove all exercise rows except the first one
    while (m_exerciseRows.size() > 1) {
        removeExerciseRow(m_exerciseRows.size() - 1);
    }
    
    // Clear the first row
    if (!m_exerciseRows.isEmpty()) {
        ExerciseRow &row = m_exerciseRows[0];
        row.exerciseCombo->setCurrentIndex(0);
        row.weightSpin->setValue(0.0);
        row.repsSpin->setValue(1);
        row.setsSpin->setValue(1);
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
        showError("Please add at least one exercise to the workout.");
        return false;
    }
    
    // Validate each exercise row
    for (int i = 0; i < m_exerciseRows.size(); ++i) {
        const ExerciseRow &row = m_exerciseRows[i];
        int exerciseId = row.exerciseCombo->currentData().toInt();
        
        if (exerciseId > 0) {
            // Exercise is selected, validate the data
            if (row.weightSpin->value() < 0) {
                showError(QString("Exercise %1: Weight cannot be negative.").arg(i + 1));
                return false;
            }
            
            if (row.repsSpin->value() <= 0) {
                showError(QString("Exercise %1: Reps must be greater than 0.").arg(i + 1));
                return false;
            }
            
            if (row.setsSpin->value() <= 0) {
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