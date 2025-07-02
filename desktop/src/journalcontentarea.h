#ifndef JOURNALCONTENTAREA_H
#define JOURNALCONTENTAREA_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QPushButton>
#include "datemanager.h"
#include "bodycompositionform.h"
#include "bodycompositionview.h"
#include "models/datamanager.h"

class JournalContentArea : public QWidget
{
    Q_OBJECT

public:
    explicit JournalContentArea(DateManager *dateManager, ::DataManager *dataManager, QWidget *parent = nullptr);

private slots:
    void onDateChanged(const QDate &newDate);
    void onDataSaved(const BodyComposition &data);
    void onCancelled();
    void onEditRequested();
    void onDeleteRequested();
    void onAddNewClicked();

private:
    void setupUI();
    void updateContent();
    void showEmptyState();
    void showBodyCompositionForm();
    void showBodyCompositionView(const BodyComposition &data);
    void loadDataForCurrentDate();

    DateManager *m_dateManager;
    ::DataManager *m_dataManager;
    QLabel *m_contentLabel;
    QStackedWidget *m_stackedWidget;
    BodyCompositionForm *m_bodyCompositionForm;
    BodyCompositionView *m_bodyCompositionView;
    QPushButton *m_addNewButton;
};

#endif // JOURNALCONTENTAREA_H 