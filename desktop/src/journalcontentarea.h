#ifndef JOURNALCONTENTAREA_H
#define JOURNALCONTENTAREA_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QStackedWidget>
#include "datemanager.h"
#include "bodycompositionform.h"

class JournalContentArea : public QWidget
{
    Q_OBJECT

public:
    explicit JournalContentArea(DateManager *dateManager, QWidget *parent = nullptr);

private slots:
    void onDateChanged(const QDate &newDate);
    void onDataSaved(const BodyComposition &data);
    void onCancelled();

private:
    void setupUI();
    void updateContent();
    void showEmptyState();
    void showBodyCompositionForm();

    DateManager *m_dateManager;
    QLabel *m_contentLabel;
    QStackedWidget *m_stackedWidget;
    BodyCompositionForm *m_bodyCompositionForm;
};

#endif // JOURNALCONTENTAREA_H 