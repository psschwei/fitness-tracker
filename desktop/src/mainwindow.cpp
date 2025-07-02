#include "mainwindow.h"
#include <QVBoxLayout>
#include <QWidget>
#include <QMenuBar>
#include <QMenu>
#include <QAction>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_dateManager(new DateManager(this))
    , m_dataManager(new ::DataManager(this))
{
    setupUI();
    setupMenuBar();
    
    // Set window properties
    setWindowTitle("Fitness Tracker");
    setMinimumSize(600, 400);
    resize(800, 600);
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