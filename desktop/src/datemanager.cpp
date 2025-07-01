#include "datemanager.h"
#include <QDate>

DateManager::DateManager(QObject *parent)
    : QObject(parent)
    , m_currentDate(QDate::currentDate())
{
}

QDate DateManager::currentDate() const
{
    return m_currentDate;
}

void DateManager::setCurrentDate(const QDate &date)
{
    if (m_currentDate != date) {
        m_currentDate = date;
        emit dateChanged(m_currentDate);
    }
}

void DateManager::goToToday()
{
    setCurrentDate(QDate::currentDate());
}

void DateManager::goToPrevious()
{
    setCurrentDate(m_currentDate.addDays(-1));
}

void DateManager::goToNext()
{
    setCurrentDate(m_currentDate.addDays(1));
}

QString DateManager::formatDate(const QDate &date) const
{
    return date.toString("ddd, MMM d, yyyy");
} 