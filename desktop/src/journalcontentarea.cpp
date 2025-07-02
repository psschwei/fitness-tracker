#include "journalcontentarea.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFrame>
#include <QStackedWidget>

JournalContentArea::JournalContentArea(DateManager *dateManager, QWidget *parent)
    : QWidget(parent)
    , m_dateManager(dateManager)
{
    setupUI();
    
    // Connect to date changes
    connect(m_dateManager, &DateManager::dateChanged, this, &JournalContentArea::onDateChanged);
    
    // Connect form signals
    connect(m_bodyCompositionForm, &BodyCompositionForm::dataSaved, this, &JournalContentArea::onDataSaved);
    connect(m_bodyCompositionForm, &BodyCompositionForm::cancelled, this, &JournalContentArea::onCancelled);
    
    // Initialize content
    updateContent();
}

void JournalContentArea::setupUI()
{
    // Create stacked widget to switch between empty state and form
    m_stackedWidget = new QStackedWidget(this);
    
    // Create empty state widget
    QWidget *emptyStateWidget = new QWidget;
    m_contentLabel = new QLabel(emptyStateWidget);
    m_contentLabel->setAlignment(Qt::AlignCenter);
    m_contentLabel->setWordWrap(false);
    m_contentLabel->setStyleSheet("QLabel { font-size: 16px; color: #666; padding: 20px; }");
    m_contentLabel->setMinimumHeight(100);
    
    QVBoxLayout *emptyLayout = new QVBoxLayout(emptyStateWidget);
    emptyLayout->setContentsMargins(240, 40, 240, 40);
    emptyLayout->addStretch();
    emptyLayout->addWidget(m_contentLabel, 0, Qt::AlignCenter);
    emptyLayout->addStretch();
    
    // Create body composition form
    m_bodyCompositionForm = new BodyCompositionForm;
    
    // Add widgets to stacked widget
    m_stackedWidget->addWidget(emptyStateWidget);
    m_stackedWidget->addWidget(m_bodyCompositionForm);
    
    // Create main layout
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(m_stackedWidget);
    
    setLayout(layout);
}

void JournalContentArea::updateContent()
{
    // For now, always show the form
    // Later we'll check if data exists for this date
    showBodyCompositionForm();
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
    m_stackedWidget->setCurrentIndex(1);
}

void JournalContentArea::onDateChanged(const QDate &newDate)
{
    updateContent();
}

void JournalContentArea::onDataSaved(const BodyComposition &data)
{
    // TODO: Save data to database
    // For now, just show a message
    showEmptyState();
}

void JournalContentArea::onCancelled()
{
    showEmptyState();
} 