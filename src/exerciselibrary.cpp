#include "exerciselibrary.h"
#include <QDebug>

ExerciseLibrary::ExerciseLibrary(DataManager *dataManager, QWidget *parent)
    : QWidget(parent)
    , m_dataManager(dataManager)
    , m_currentExerciseId(0)
    , m_isEditing(false)
{
    setupUI();
    populateTable();
}

void ExerciseLibrary::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // Form section
    QGridLayout *formLayout = new QGridLayout();
    
    formLayout->addWidget(new QLabel("Exercise Name:"), 0, 0);
    m_nameEdit = new QLineEdit();
    m_nameEdit->setPlaceholderText("Enter exercise name...");
    formLayout->addWidget(m_nameEdit, 0, 1);
    
    formLayout->addWidget(new QLabel("Category:"), 1, 0);
    m_categoryCombo = new QComboBox();
    m_categoryCombo->addItems({"strength", "cardio", "flexibility"});
    formLayout->addWidget(m_categoryCombo, 1, 1);
    
    mainLayout->addLayout(formLayout);
    
    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    
    m_addButton = new QPushButton("Add Exercise");
    m_addButton->setStyleSheet("QPushButton { padding: 8px; font-weight: bold; background-color: #4CAF50; color: white; border: none; border-radius: 4px; }");
    connect(m_addButton, &QPushButton::clicked, this, &ExerciseLibrary::addExercise);
    
    m_editButton = new QPushButton("Update Exercise");
    m_editButton->setStyleSheet("QPushButton { padding: 8px; font-weight: bold; background-color: #2196F3; color: white; border: none; border-radius: 4px; }");
    m_editButton->setEnabled(false);
    connect(m_editButton, &QPushButton::clicked, this, &ExerciseLibrary::editExercise);
    
    m_deleteButton = new QPushButton("Delete Exercise");
    m_deleteButton->setStyleSheet("QPushButton { padding: 8px; font-weight: bold; background-color: #f44336; color: white; border: none; border-radius: 4px; }");
    m_deleteButton->setEnabled(false);
    connect(m_deleteButton, &QPushButton::clicked, this, &ExerciseLibrary::deleteExercise);
    
    m_clearButton = new QPushButton("Clear Form");
    m_clearButton->setStyleSheet("QPushButton { padding: 8px; font-weight: bold; background-color: #9E9E9E; color: white; border: none; border-radius: 4px; }");
    connect(m_clearButton, &QPushButton::clicked, this, &ExerciseLibrary::clearForm);
    
    buttonLayout->addWidget(m_addButton);
    buttonLayout->addWidget(m_editButton);
    buttonLayout->addWidget(m_deleteButton);
    buttonLayout->addWidget(m_clearButton);
    
    mainLayout->addLayout(buttonLayout);
    
    // Table
    setupTable();
    mainLayout->addWidget(m_exercisesTable);
}

void ExerciseLibrary::setupTable()
{
    m_exercisesTable = new QTableWidget();
    m_exercisesTable->setColumnCount(4);
    m_exercisesTable->setHorizontalHeaderLabels({"ID", "Name", "Category", "Status"});
    m_exercisesTable->horizontalHeader()->setStretchLastSection(true);
    m_exercisesTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_exercisesTable->setSelectionMode(QAbstractItemView::SingleSelection);
    
    connect(m_exercisesTable, &QTableWidget::itemSelectionChanged, [this]() {
        QList<QTableWidgetItem*> selectedItems = m_exercisesTable->selectedItems();
        if (!selectedItems.isEmpty()) {
            int row = selectedItems.first()->row();
            int exerciseId = m_exercisesTable->item(row, 0)->text().toInt();
            
            Exercise exercise = m_dataManager->loadExercise(exerciseId);
            if (exercise.id() > 0) {
                m_currentExerciseId = exercise.id();
                m_nameEdit->setText(exercise.name());
                m_categoryCombo->setCurrentText(exercise.category());
                
                m_addButton->setEnabled(false);
                m_editButton->setEnabled(true);
                m_deleteButton->setEnabled(true);
                m_isEditing = true;
            }
        }
    });
}

void ExerciseLibrary::populateTable()
{
    m_exercisesTable->setRowCount(0);
    
    QList<Exercise> exercises = m_dataManager->getAllExercises();
    
    for (const Exercise &exercise : exercises) {
        int row = m_exercisesTable->rowCount();
        m_exercisesTable->insertRow(row);
        
        m_exercisesTable->setItem(row, 0, new QTableWidgetItem(QString::number(exercise.id())));
        m_exercisesTable->setItem(row, 1, new QTableWidgetItem(exercise.name()));
        m_exercisesTable->setItem(row, 2, new QTableWidgetItem(exercise.category()));
        m_exercisesTable->setItem(row, 3, new QTableWidgetItem(exercise.isActive() ? "Active" : "Inactive"));
    }
}

void ExerciseLibrary::addExercise()
{
    if (!validateForm()) {
        return;
    }
    
    Exercise exercise(0, m_nameEdit->text().trimmed(), m_categoryCombo->currentText());
    
    if (m_dataManager->saveExercise(exercise)) {
        showSuccess("Exercise added successfully!");
        emit exerciseAdded();
        clearForm();
        populateTable();
    } else {
        showError("Failed to add exercise. Please check your data.");
    }
}

void ExerciseLibrary::editExercise()
{
    if (!validateForm() || m_currentExerciseId == 0) {
        return;
    }
    
    Exercise exercise = m_dataManager->loadExercise(m_currentExerciseId);
    if (exercise.id() == 0) {
        showError("Exercise not found.");
        return;
    }
    
    exercise.setName(m_nameEdit->text().trimmed());
    exercise.setCategory(m_categoryCombo->currentText());
    
    if (m_dataManager->saveExercise(exercise)) {
        showSuccess("Exercise updated successfully!");
        emit exerciseUpdated();
        clearForm();
        populateTable();
    } else {
        showError("Failed to update exercise. Please check your data.");
    }
}

void ExerciseLibrary::deleteExercise()
{
    if (m_currentExerciseId == 0) {
        return;
    }
    
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, 
        "Delete Exercise", 
        "Are you sure you want to delete this exercise? This action cannot be undone.",
        QMessageBox::Yes | QMessageBox::No
    );
    
    if (reply == QMessageBox::Yes) {
        if (m_dataManager->deleteExercise(m_currentExerciseId)) {
            showSuccess("Exercise deleted successfully!");
            emit exerciseDeleted();
            clearForm();
            populateTable();
        } else {
            showError("Failed to delete exercise.");
        }
    }
}

void ExerciseLibrary::clearForm()
{
    m_nameEdit->clear();
    m_categoryCombo->setCurrentIndex(0);
    m_currentExerciseId = 0;
    m_isEditing = false;
    
    m_addButton->setEnabled(true);
    m_editButton->setEnabled(false);
    m_deleteButton->setEnabled(false);
    
    m_exercisesTable->clearSelection();
}

void ExerciseLibrary::refreshTable()
{
    populateTable();
}

bool ExerciseLibrary::validateForm()
{
    if (m_nameEdit->text().trimmed().isEmpty()) {
        showError("Exercise name cannot be empty.");
        return false;
    }
    
    if (m_nameEdit->text().trimmed().length() > 255) {
        showError("Exercise name is too long (max 255 characters).");
        return false;
    }
    
    return true;
}

void ExerciseLibrary::showError(const QString &message)
{
    QMessageBox::warning(this, "Validation Error", message);
}

void ExerciseLibrary::showSuccess(const QString &message)
{
    QMessageBox::information(this, "Success", message);
} 