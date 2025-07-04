#include <QApplication>
#include <QStyleFactory>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // Set application properties
    app.setApplicationName("Fitness Tracker");
    app.setApplicationVersion("0.1.0");
    app.setOrganizationName("Fitness Tracker");
    
    // Use system theme
    app.setStyle(QStyleFactory::create("Fusion"));
    
    // Create and show main window
    MainWindow window;
    window.show();
    
    return app.exec();
} 