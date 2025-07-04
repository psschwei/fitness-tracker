#include "datenavigationbar.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDate>

DateNavigationBar::DateNavigationBar(DateManager *dateManager, QWidget *parent)
    : QWidget(parent)
    , m_dateManager(dateManager)
{
    setupUI();
    
    // Connect signals
    connect(m_dateManager, &DateManager::dateChanged, this, &DateNavigationBar::onDateChanged);
    connect(m_dateEdit, &QDateEdit::dateChanged, this, &DateNavigationBar::onDateEditChanged);
    connect(m_todayButton, &QPushButton::clicked, this, &DateNavigationBar::onTodayClicked);
    connect(m_previousButton, &QPushButton::clicked, this, &DateNavigationBar::onPreviousClicked);
    connect(m_nextButton, &QPushButton::clicked, this, &DateNavigationBar::onNextClicked);
    
    // Initialize with current date
    updateDateDisplay();
}

void DateNavigationBar::setupUI()
{
    // Create widgets
    m_dateEdit = new QDateEdit(this);
    m_dateEdit->setCalendarPopup(true);
    m_dateEdit->setDisplayFormat("MM/dd/yyyy");
    m_dateEdit->setReadOnly(true);
    m_dateEdit->setButtonSymbols(QAbstractSpinBox::NoButtons);
    m_dateEdit->setFocusPolicy(Qt::NoFocus);
    
    m_todayButton = new QPushButton("Today", this);
    m_previousButton = new QPushButton("←", this);
    m_nextButton = new QPushButton("→", this);
    m_dateLabel = new QLabel(this);
    
    // Set button sizes
    m_previousButton->setFixedSize(40, 30);
    m_nextButton->setFixedSize(40, 30);
    m_todayButton->setFixedHeight(30);
    m_dateEdit->setFixedHeight(30);
    
    // Create layout
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(10, 10, 10, 10);
    layout->setSpacing(10);
    
    layout->addWidget(new QLabel("Date:"));
    layout->addWidget(m_dateEdit);
    layout->addWidget(m_todayButton);
    layout->addWidget(m_previousButton);
    layout->addWidget(m_nextButton);
    layout->addStretch();
    layout->addWidget(m_dateLabel);
    
    setLayout(layout);

    // Set initial focus to the Today button
    m_todayButton->setFocus();
}

void DateNavigationBar::updateDateDisplay()
{
    QDate currentDate = m_dateManager->currentDate();
    m_dateEdit->setDate(currentDate);
    m_dateLabel->setText(m_dateManager->formatDate(currentDate));
}

void DateNavigationBar::onDateChanged(const QDate &newDate)
{
    // Block signals to prevent recursive calls
    m_dateEdit->blockSignals(true);
    m_dateEdit->setDate(newDate);
    m_dateEdit->blockSignals(false);
    
    m_dateLabel->setText(m_dateManager->formatDate(newDate));
}

void DateNavigationBar::onDateEditChanged(const QDate &date)
{
    m_dateManager->setCurrentDate(date);
}

void DateNavigationBar::onTodayClicked()
{
    m_dateManager->goToToday();
}

void DateNavigationBar::onPreviousClicked()
{
    m_dateManager->goToPrevious();
}

void DateNavigationBar::onNextClicked()
{
    m_dateManager->goToNext();
} 