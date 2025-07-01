#ifndef JOURNALCONTENTAREA_H
#define JOURNALCONTENTAREA_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include "datemanager.h"

class JournalContentArea : public QWidget
{
    Q_OBJECT

public:
    explicit JournalContentArea(DateManager *dateManager, QWidget *parent = nullptr);

private slots:
    void onDateChanged(const QDate &newDate);

private:
    void setupUI();
    void updateContent();

    DateManager *m_dateManager;
    QLabel *m_contentLabel;
};

#endif // JOURNALCONTENTAREA_H 