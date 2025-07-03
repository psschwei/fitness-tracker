#include "mainwindow.h"
#include <QApplication>
#include <QVBoxLayout>
#include <QWidget>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QKeyEvent>
#include <QShortcut>
#include <QLineEdit>
#include <QTextEdit>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_dateManager(new DateManager(this))
    , m_dataManager(new ::DataManager(this))
{
    setupUI();
    setupMenuBar();
    
    // Set window properties
    setWindowTitle("Fitness Tracker");
    setMinimumSize(600, 600);
    resize(800, 1000);

    // Add left/right arrow shortcuts for day navigation
    QShortcut *leftShortcut = new QShortcut(QKeySequence(Qt::Key_Left), this);
    connect(leftShortcut, &QShortcut::activated, this, [this]() {
        QWidget *fw = QApplication::focusWidget();
        if (qobject_cast<QLineEdit*>(fw) || qobject_cast<QTextEdit*>(fw)) return;
        m_dateManager->goToPrevious();
    });
    QShortcut *rightShortcut = new QShortcut(QKeySequence(Qt::Key_Right), this);
    connect(rightShortcut, &QShortcut::activated, this, [this]() {
        QWidget *fw = QApplication::focusWidget();
        if (qobject_cast<QLineEdit*>(fw) || qobject_cast<QTextEdit*>(fw)) return;
        m_dateManager->goToNext();
    });
}

void MainWindow::setupUI()
{
    // Create central widget
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    // Create main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    
    // Create components
    m_dateNavigationBar = new DateNavigationBar(m_dateManager, this);
    m_journalContentArea = new JournalContentArea(m_dateManager, m_dataManager, this);
    
    // Add to layout
    mainLayout->addWidget(m_dateNavigationBar);
    mainLayout->addWidget(m_journalContentArea, 1); // Give content area more space
}

void MainWindow::setupMenuBar()
{
    QMenuBar *menuBar = this->menuBar();
    
    // File menu
    QMenu *fileMenu = menuBar->addMenu("&File");
    QAction *exitAction = fileMenu->addAction("E&xit");
    connect(exitAction, &QAction::triggered, this, &QWidget::close);
    
    // Help menu
    QMenu *helpMenu = menuBar->addMenu("&Help");
    QAction *aboutAction = helpMenu->addAction("&About");
    // TODO: Add about dialog
} 