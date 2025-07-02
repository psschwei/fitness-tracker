#include "journalcontentarea.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFrame>
#include <QStackedWidget>
#include <QMessageBox>

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
    
    // Initialize content
    updateContent();
}

void JournalContentArea::setupUI()
{
    // Create stacked widget to switch between different states
    m_stackedWidget = new QStackedWidget(this);
    
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
    
    // Create main layout
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(m_stackedWidget);
    
    setLayout(layout);
}

void JournalContentArea::updateContent()
{
    loadDataForCurrentDate();
}

void JournalContentArea::loadDataForCurrentDate()
{
    QDate currentDate = m_dateManager->currentDate();
    
    if (m_dataManager->hasBodyComposition(currentDate)) {
        BodyComposition data = m_dataManager->loadBodyComposition(currentDate);
        showBodyCompositionView(data);
    } else {
        showEmptyState();
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