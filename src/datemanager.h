#ifndef DATEMANAGER_H
#define DATEMANAGER_H

#include <QObject>
#include <QDate>

class DateManager : public QObject
{
    Q_OBJECT

public:
    explicit DateManager(QObject *parent = nullptr);
    
    QDate currentDate() const;
    void setCurrentDate(const QDate &date);
    
    void goToToday();
    void goToPrevious();
    void goToNext();
    
    QString formatDate(const QDate &date) const;

signals:
    void dateChanged(const QDate &newDate);

private:
    QDate m_currentDate;
};

#endif // DATEMANAGER_H 