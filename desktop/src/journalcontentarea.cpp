#include "journalcontentarea.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFrame>

JournalContentArea::JournalContentArea(DateManager *dateManager, QWidget *parent)
    : QWidget(parent)
    , m_dateManager(dateManager)
{
    setupUI();
    
    // Connect to date changes
    connect(m_dateManager, &DateManager::dateChanged, this, &JournalContentArea::onDateChanged);
    
    // Initialize content
    updateContent();
}

void JournalContentArea::setupUI()
{
    // Create main content label
    m_contentLabel = new QLabel(this);
    m_contentLabel->setAlignment(Qt::AlignCenter);
    m_contentLabel->setWordWrap(false);  // Disable word wrap to keep text on one line
    m_contentLabel->setStyleSheet("QLabel { font-size: 16px; color: #666; padding: 20px; }");
    m_contentLabel->setMinimumHeight(100);
    
    // Create layout
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(240, 40, 240, 40);  // More left/right margin
    layout->addStretch();
    layout->addWidget(m_contentLabel, 0, Qt::AlignCenter);
    layout->addStretch();
    
    setLayout(layout);
}

void JournalContentArea::updateContent()
{
    QDate currentDate = m_dateManager->currentDate();
    QString formattedDate = m_dateManager->formatDate(currentDate);
    m_contentLabel->setText(QString("No data for %1").arg(formattedDate));
}

void JournalContentArea::onDateChanged(const QDate &newDate)
{
    updateContent();
} 