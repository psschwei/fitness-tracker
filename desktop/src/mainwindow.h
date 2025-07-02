#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include "datemanager.h"
#include "datenavigationbar.h"
#include "journalcontentarea.h"
#include "models/datamanager.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    void setupUI();
    void setupMenuBar();

    DateManager *m_dateManager;
    ::DataManager *m_dataManager;
    DateNavigationBar *m_dateNavigationBar;
    JournalContentArea *m_journalContentArea;
};

#endif // MAINWINDOW_H 