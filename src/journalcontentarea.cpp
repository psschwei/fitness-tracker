#include "journalcontentarea.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFrame>
#include <QStackedWidget>
#include <QMessageBox>
#include <QShortcut>
#include <QKeySequence>

JournalContentArea::JournalContentArea(DateManager *dateManager, ::DataManager *dataManager, QWidget *parent)
    : QWidget(parent)
    , m_dateManager(dateManager)
    , m_dataManager(dataManager)
{
    setupUI();
    
    // Connect to date changes
    connect(m_dateManager, &DateManager::dateChanged, this, &JournalContentArea::onDateChanged);
    
    // Connect form signals
    connect(m_bodyCompositionForm, &BodyCompositionForm::dataSaved, this, &JournalContentArea::onDataSaved);
    connect(m_bodyCompositionForm, &BodyCompositionForm::cancelled, this, &JournalContentArea::onCancelled);
    
    // Connect view signals
    connect(m_bodyCompositionView, &BodyCompositionView::editRequested, this, &JournalContentArea::onEditRequested);
    connect(m_bodyCompositionView, &BodyCompositionView::deleteRequested, this, &JournalContentArea::onDeleteRequested);
    
    // Connect add new button
    connect(m_addNewButton, &QPushButton::clicked, this, &JournalContentArea::onAddNewClicked);
    
    // Connect exercise tracking signals
    connect(m_workoutBuilder, &WorkoutBuilder::workoutCreated, this, &JournalContentArea::onWorkoutCreated);
    connect(m_workoutBuilder, &WorkoutBuilder::cancelled, this, &JournalContentArea::onWorkoutCancelled);
    connect(m_workoutView, &WorkoutView::editRequested, this, &JournalContentArea::onWorkoutEditRequested);
    connect(m_workoutView, &WorkoutView::deleteRequested, this, &JournalContentArea::onWorkoutDeleteRequested);
    connect(m_exerciseLibrary, &ExerciseLibrary::exerciseAdded, this, &JournalContentArea::onExerciseAdded);
    connect(m_exerciseLibrary, &ExerciseLibrary::exerciseUpdated, this, &JournalContentArea::onExerciseUpdated);
    connect(m_exerciseLibrary, &ExerciseLibrary::exerciseDeleted, this, &JournalContentArea::onExerciseDeleted);
    
    // Setup keyboard shortcuts for tabs
    setupKeyboardShortcuts();
    
    // Initialize content
    updateContent();
}

void JournalContentArea::setupUI()
{
    // Create tab widget for different sections
    m_tabWidget = new QTabWidget(this);
    
    // Create stacked widget for body composition
    m_stackedWidget = new QStackedWidget();
    
    // Create empty state widget
    QWidget *emptyStateWidget = new QWidget;
    m_contentLabel = new QLabel(emptyStateWidget);
    m_contentLabel->setAlignment(Qt::AlignCenter);
    m_contentLabel->setWordWrap(false);
    m_contentLabel->setStyleSheet("QLabel { font-size: 16px; color: #666; padding: 20px; }");
    m_contentLabel->setMinimumHeight(100);
    
    m_addNewButton = new QPushButton("Add Body Composition", emptyStateWidget);
    m_addNewButton->setStyleSheet("QPushButton { background-color: #007bff; color: white; border: none; padding: 10px 20px; border-radius: 4px; font-size: 14px; }");
    
    QVBoxLayout *emptyLayout = new QVBoxLayout(emptyStateWidget);
    emptyLayout->setContentsMargins(240, 40, 240, 40);
    emptyLayout->addStretch();
    emptyLayout->addWidget(m_contentLabel, 0, Qt::AlignCenter);
    emptyLayout->addWidget(m_addNewButton, 0, Qt::AlignCenter);
    emptyLayout->addStretch();
    
    // Create body composition form
    m_bodyCompositionForm = new BodyCompositionForm;
    
    // Create body composition view
    m_bodyCompositionView = new BodyCompositionView;
    
    // Add widgets to stacked widget
    m_stackedWidget->addWidget(emptyStateWidget);      // Index 0: Empty state
    m_stackedWidget->addWidget(m_bodyCompositionForm); // Index 1: Form
    m_stackedWidget->addWidget(m_bodyCompositionView); // Index 2: View
    
    // Create exercise tracking components
    m_workoutBuilder = new WorkoutBuilder(m_dataManager);
    m_workoutView = new WorkoutView(m_dataManager);
    m_exerciseLibrary = new ExerciseLibrary(m_dataManager);
    
    // Create analytics tab
    m_analyticsTab = new AnalyticsTab(m_dataManager);
    
    // Create stacked widget for workouts
    m_workoutStackedWidget = new QStackedWidget();

    // Create empty state widget for workouts
    m_workoutEmptyStateWidget = new QWidget;
    QLabel *workoutEmptyLabel = new QLabel("No workout for this date");
    workoutEmptyLabel->setAlignment(Qt::AlignCenter);
    workoutEmptyLabel->setStyleSheet("QLabel { font-size: 16px; color: #666; padding: 20px; }");
    QPushButton *addWorkoutButton = new QPushButton("Add Workout", m_workoutEmptyStateWidget);
    addWorkoutButton->setStyleSheet("QPushButton { background-color: #007bff; color: white; border: none; padding: 10px 20px; border-radius: 4px; font-size: 14px; }");
    QVBoxLayout *workoutEmptyLayout = new QVBoxLayout(m_workoutEmptyStateWidget);
    workoutEmptyLayout->setContentsMargins(240, 40, 240, 40);
    workoutEmptyLayout->addStretch();
    workoutEmptyLayout->addWidget(workoutEmptyLabel, 0, Qt::AlignCenter);
    workoutEmptyLayout->addWidget(addWorkoutButton, 0, Qt::AlignCenter);
    workoutEmptyLayout->addStretch();
    connect(addWorkoutButton, &QPushButton::clicked, this, &JournalContentArea::onAddWorkoutClicked);

    m_workoutStackedWidget->addWidget(m_workoutEmptyStateWidget); // Index 0: Empty state
    m_workoutStackedWidget->addWidget(m_workoutView);      // Index 1: View
    m_workoutStackedWidget->addWidget(m_workoutBuilder);   // Index 2: Form
    
    // Add tabs
    m_tabWidget->addTab(m_stackedWidget, "Body Composition");
    m_tabWidget->addTab(m_workoutStackedWidget, "Workouts");
    m_tabWidget->addTab(m_exerciseLibrary, "Exercise Library");
    m_tabWidget->addTab(m_analyticsTab, "Visualizations");
    
    // Create main layout
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(m_tabWidget);
    
    setLayout(layout);
}

void JournalContentArea::updateContent()
{
    loadDataForCurrentDate();
}

void JournalContentArea::loadDataForCurrentDate()
{
    QDate currentDate = m_dateManager->currentDate();
    
    // Load body composition data
    if (m_dataManager->hasBodyComposition(currentDate)) {
        BodyComposition data = m_dataManager->loadBodyComposition(currentDate);
        showBodyCompositionView(data);
    } else {
        showEmptyState();
    }
    
    // Load workout data
    QList<Workout> workouts = m_dataManager->getWorkoutsByDate(currentDate);
    m_workoutView->setDate(currentDate);
    if (workouts.isEmpty()) {
        m_workoutStackedWidget->setCurrentIndex(0); // Show empty state
    } else {
        m_workoutStackedWidget->setCurrentIndex(1); // Show view
    }
}

void JournalContentArea::showEmptyState()
{
    QDate currentDate = m_dateManager->currentDate();
    QString formattedDate = m_dateManager->formatDate(currentDate);
    m_contentLabel->setText(QString("No data for %1").arg(formattedDate));
    m_stackedWidget->setCurrentIndex(0);
}

void JournalContentArea::showBodyCompositionForm()
{
    m_bodyCompositionForm->setDate(m_dateManager->currentDate());
    m_bodyCompositionForm->clear();
    
    // Try to prefill with yesterday's data
    QDate yesterday = m_dateManager->currentDate().addDays(-1);
    if (m_dataManager->hasBodyComposition(yesterday)) {
        BodyComposition yesterdayData = m_dataManager->loadBodyComposition(yesterday);
        m_bodyCompositionForm->prefillWithData(yesterdayData);
    }
    
    m_stackedWidget->setCurrentIndex(1);
}

void JournalContentArea::showBodyCompositionView(const BodyComposition &data)
{
    m_bodyCompositionView->setData(data);
    m_stackedWidget->setCurrentIndex(2);
}

void JournalContentArea::onDateChanged(const QDate &newDate)
{
    updateContent();
}

void JournalContentArea::onDataSaved(const BodyComposition &data)
{
    if (m_dataManager->saveBodyComposition(data)) {
        showBodyCompositionView(data);
    } else {
        QMessageBox::critical(this, "Error", "Failed to save data. Please try again.");
    }
}

void JournalContentArea::onCancelled()
{
    loadDataForCurrentDate();
}

void JournalContentArea::onEditRequested()
{
    m_bodyCompositionForm->setDate(m_dateManager->currentDate());
    
    // Load current data for editing
    QDate currentDate = m_dateManager->currentDate();
    if (m_dataManager->hasBodyComposition(currentDate)) {
        BodyComposition currentData = m_dataManager->loadBodyComposition(currentDate);
        m_bodyCompositionForm->setData(currentData);
    }
    
    m_stackedWidget->setCurrentIndex(1);
}

void JournalContentArea::onDeleteRequested()
{
    QDate currentDate = m_dateManager->currentDate();
    m_dataManager->deleteBodyComposition(currentDate);
    showEmptyState();
}

void JournalContentArea::onAddNewClicked()
{
    showBodyCompositionForm();
}

void JournalContentArea::onWorkoutCreated()
{
    // Refresh workout view and switch to view mode
    m_workoutView->refreshData();
    m_workoutStackedWidget->setCurrentIndex(1);
}

void JournalContentArea::onWorkoutEditRequested()
{
    // Switch to workout builder for editing
    m_workoutBuilder->setDate(m_dateManager->currentDate());
    
    // Load existing workout data for editing
    QList<Workout> workouts = m_dataManager->getWorkoutsByDate(m_dateManager->currentDate());
    m_workoutBuilder->loadWorkoutData(workouts);
    
    m_workoutStackedWidget->setCurrentIndex(2);
}

void JournalContentArea::onWorkoutDeleteRequested()
{
    // Refresh workout view after deletion
    m_workoutView->refreshData();
}

void JournalContentArea::onWorkoutCancelled()
{
    // Switch back to workout view
    m_workoutView->refreshData();
    m_workoutStackedWidget->setCurrentIndex(1);
}

void JournalContentArea::onExerciseAdded()
{
    // Refresh workout builder with new exercises
    m_workoutBuilder->updateExerciseComboBox();
}

void JournalContentArea::onExerciseUpdated()
{
    // Refresh workout builder with updated exercises
    m_workoutBuilder->updateExerciseComboBox();
}

void JournalContentArea::onExerciseDeleted()
{
    // Refresh workout builder with updated exercises
    m_workoutBuilder->updateExerciseComboBox();
}

void JournalContentArea::onAddWorkoutClicked()
{
    m_workoutBuilder->setDate(m_dateManager->currentDate());
    m_workoutBuilder->clearForm();
    m_workoutStackedWidget->setCurrentIndex(2);
}

void JournalContentArea::setupKeyboardShortcuts()
{
    // Body Composition tab - 'b' key
    QShortcut *bodyCompShortcut = new QShortcut(QKeySequence("b"), this);
    connect(bodyCompShortcut, &QShortcut::activated, this, &JournalContentArea::switchToBodyComposition);
    
    // Workouts tab - 'w' key
    QShortcut *workoutsShortcut = new QShortcut(QKeySequence("w"), this);
    connect(workoutsShortcut, &QShortcut::activated, this, &JournalContentArea::switchToWorkouts);
    
    // Exercise Library tab - 'l' key
    QShortcut *exerciseLibraryShortcut = new QShortcut(QKeySequence("l"), this);
    connect(exerciseLibraryShortcut, &QShortcut::activated, this, &JournalContentArea::switchToExerciseLibrary);
    
    // Visualizations tab - 'v' key
    QShortcut *visualizationsShortcut = new QShortcut(QKeySequence("v"), this);
    connect(visualizationsShortcut, &QShortcut::activated, this, &JournalContentArea::switchToVisualizations);
    
    // Action shortcuts
    // Edit - 'e' key
    QShortcut *editShortcut = new QShortcut(QKeySequence("e"), this);
    connect(editShortcut, &QShortcut::activated, this, &JournalContentArea::onEditShortcut);
    
    // Delete - 'd' key
    QShortcut *deleteShortcut = new QShortcut(QKeySequence("d"), this);
    connect(deleteShortcut, &QShortcut::activated, this, &JournalContentArea::onDeleteShortcut);
    
    // Add - 'a' key
    QShortcut *addShortcut = new QShortcut(QKeySequence("a"), this);
    connect(addShortcut, &QShortcut::activated, this, &JournalContentArea::onAddShortcut);
    
    // Navigation shortcuts
    // Today - 't' key
    QShortcut *todayShortcut = new QShortcut(QKeySequence("t"), this);
    connect(todayShortcut, &QShortcut::activated, this, &JournalContentArea::onTodayShortcut);
    
    // Next day - 'n' key
    QShortcut *nextDayShortcut = new QShortcut(QKeySequence("n"), this);
    connect(nextDayShortcut, &QShortcut::activated, this, &JournalContentArea::onNextDayShortcut);
    
    // Previous day - 'p' key
    QShortcut *prevDayShortcut = new QShortcut(QKeySequence("p"), this);
    connect(prevDayShortcut, &QShortcut::activated, this, &JournalContentArea::onPrevDayShortcut);
}

void JournalContentArea::switchToBodyComposition()
{
    m_tabWidget->setCurrentIndex(0);
}

void JournalContentArea::switchToWorkouts()
{
    m_tabWidget->setCurrentIndex(1);
}

void JournalContentArea::switchToExerciseLibrary()
{
    m_tabWidget->setCurrentIndex(2);
}

void JournalContentArea::switchToVisualizations()
{
    m_tabWidget->setCurrentIndex(3);
}

void JournalContentArea::onEditShortcut()
{
    int currentTab = m_tabWidget->currentIndex();
    
    if (currentTab == 0) { // Body Composition tab
        // Check if we're in view mode (not form mode)
        if (m_stackedWidget->currentIndex() == 2) { // View mode
            onEditRequested();
        }
    } else if (currentTab == 1) { // Workouts tab
        // Check if we're in view mode (not builder mode)
        if (m_workoutStackedWidget->currentIndex() == 1) { // View mode
            onWorkoutEditRequested();
        }
    }
}

void JournalContentArea::onDeleteShortcut()
{
    int currentTab = m_tabWidget->currentIndex();
    
    if (currentTab == 0) { // Body Composition tab
        // Check if we're in view mode (not form mode)
        if (m_stackedWidget->currentIndex() == 2) { // View mode
            onDeleteRequested();
        }
    } else if (currentTab == 1) { // Workouts tab
        // Check if we're in view mode (not builder mode)
        if (m_workoutStackedWidget->currentIndex() == 1) { // View mode
            onWorkoutDeleteRequested();
        }
    }
}

void JournalContentArea::onAddShortcut()
{
    int currentTab = m_tabWidget->currentIndex();
    
    if (currentTab == 0) { // Body Composition tab
        // Check if we're in empty state
        if (m_stackedWidget->currentIndex() == 0) { // Empty state
            onAddNewClicked();
        }
    } else if (currentTab == 1) { // Workouts tab
        // Check if we're in empty state
        if (m_workoutStackedWidget->currentIndex() == 0) { // Empty state
            onAddWorkoutClicked();
        }
    }
}

void JournalContentArea::onNextDayShortcut()
{
    m_dateManager->goToNext();
}

void JournalContentArea::onTodayShortcut()
{
    m_dateManager->goToToday();
}

void JournalContentArea::onPrevDayShortcut()
{
    m_dateManager->goToPrevious();
} 