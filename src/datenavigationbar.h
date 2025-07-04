#ifndef DATENAVIGATIONBAR_H
#define DATENAVIGATIONBAR_H

#include <QWidget>
#include <QDateEdit>
#include <QPushButton>
#include <QLabel>
#include "datemanager.h"

class DateNavigationBar : public QWidget
{
    Q_OBJECT

public:
    explicit DateNavigationBar(DateManager *dateManager, QWidget *parent = nullptr);

private slots:
    void onDateChanged(const QDate &newDate);
    void onDateEditChanged(const QDate &date);
    void onTodayClicked();
    void onPreviousClicked();
    void onNextClicked();

private:
    void setupUI();
    void updateDateDisplay();

    DateManager *m_dateManager;
    QDateEdit *m_dateEdit;
    QPushButton *m_todayButton;
    QPushButton *m_previousButton;
    QPushButton *m_nextButton;
    QLabel *m_dateLabel;
};

#endif // DATENAVIGATIONBAR_H 