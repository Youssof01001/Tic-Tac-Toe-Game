#include <QApplication>
#include "../includes/mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Set application properties
    app.setApplicationName("Tic Tac Toe");
    app.setApplicationVersion("2.0");
    app.setOrganizationName("GameStudio");

    // Create and show main window (now contains everything)
    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}
