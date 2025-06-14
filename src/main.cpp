// Written by malekpour-dev.ir
// Main entry point for the application
// It initializes the application, sets up styles, and shows the main window.

#include <QApplication>
#include <QStyleFactory>
#include <QDir>
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    app.setApplicationName("twiq");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("malekpour dev");
    app.setOrganizationDomain("malekpour-dev.ir");
    app.setDesktopFileName("twiq");
    
    // dark mode style
    // comment to change to light mode
    app.setStyleSheet(
            "QToolTip { "
            "    color: #ffffff; "
            "    background-color: #2a2a2a; "
            "    border: 1px solid white; "
            "}"
            "QGroupBox { "
            "    font-weight: bold; "
            "    border: 2px solid #555555; "
            "    border-radius: 5px; "
            "    margin-top: 1ex; "
            "    padding-top: 5px; "
            "}"
            "QGroupBox::title { "
            "    subcontrol-origin: margin; "
            "    left: 10px; "
            "    padding: 0 5px 0 5px; "
            "}"
            "QPushButton { "
            "    border: 2px solid #555555; "
            "    border-radius: 4px; "
            "    padding: 4px 8px; "
            "    min-width: 60px; "
            "}"
            "QPushButton:hover { "
            "    border-color: #2a82da; "
            "}"
            "QPushButton:pressed { "
            "    background-color: #404040; "
            "}"
            "QSlider::groove:horizontal { "
            "    border: 1px solid #999999; "
            "    height: 6px; "
            "    background: #353535; "
            "    margin: 2px 0; "
            "    border-radius: 3px; "
            "}"
            "QSlider::handle:horizontal { "
            "    background: #2a82da; "
            "    border: 1px solid #2a82da; "
            "    width: 16px; "
            "    margin: -2px 0; "
            "    border-radius: 8px; "
            "}"
            "QListWidget { "
            "    border: 1px solid #555555; "
            "    border-radius: 4px; "
            "    background-color: #2a2a2a; "
            "    selection-background-color: #2a82da; "
            "}"
            "QListWidget::item { "
            "    padding: 4px; "
            "    border-bottom: 1px solid #404040; "
            "}"
            "QListWidget::item:selected { "
            "    background-color: #2a82da; "
            "}"
            "QComboBox { "
            "    border: 1px solid #555555; "
            "    border-radius: 4px; "
            "    padding: 2px 8px; "
            "    min-width: 80px; "
            "}"
            "QComboBox::drop-down { "
            "    border-left: 1px solid #555555; "
            "    width: 20px; "
            "}"
            "QLineEdit { "
            "    border: 1px solid #555555; "
            "    border-radius: 4px; "
            "    padding: 2px 4px; "
            "}"
    );
    
    MainWindow window;
    // force to maximize in all platforms
    QTimer::singleShot(50, &window, [&window]() {
        window.setWindowState(Qt::WindowMaximized);
    });
    window.show();

    qDebug() << "App started";

    return app.exec();
}