#include <QApplication>
#include <QStyleFactory>
#include <QDir>
#include "LoginDialog.h"
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Set application properties
    app.setApplicationName("Tic Tac Toe");
    app.setApplicationVersion("2.0");
    app.setOrganizationName("GameStudio");

    // Create and show login dialog
    LoginDialog loginDialog;
    if (loginDialog.exec() == QDialog::Accepted) {
        // Get login information
        QString player1 = loginDialog.getPlayer1Name();
        QString player2 = loginDialog.getPlayer2Name();
        QString gameMode = loginDialog.getGameMode();

        // Create and show main window
        MainWindow* mainWindow = new MainWindow();
        mainWindow->setPlayers(player1, player2, gameMode);
        mainWindow->show();

        return app.exec();
    }

    return 0;
}
