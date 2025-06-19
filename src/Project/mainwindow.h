#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QToolBar>
#include <QAction>
#include <QMessageBox>
#include <QDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QTimer>
#include <QPixmap>
#include <QPalette>
#include <QResizeEvent>
#include <QApplication>
#include <QPropertyAnimation>
#include <QRect>
#include "Board.h"
#include "AIPlayer.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() = default;

    void setPlayers(const QString& p1, const QString& p2, const QString& mode);

protected:
    void resizeEvent(QResizeEvent* event) override;

private slots:
    void onGameButtonClicked();
    void onNewGameClicked();
    void onShowHistoryClicked();
    void onLogoutClicked();
    void makeAIMove();

private:
    void setupUI();
    void setupStyling();
    void setBackgroundImage();
    void setupGameGrid();
    void setupToolbar();
    void resetGame();
    void updateGameStatus();
    void checkGameEnd();
    void saveGameHistory(const QString& winner);
    void showGameOverDialog(const QString& result);
    void animateButton(QPushButton* button);  // Declaration added here

    // UI Components
    QWidget* centralWidget;
    QVBoxLayout* mainLayout;
    QLabel* titleLabel;
    QLabel* statusLabel;
    QLabel* playersLabel;
    QGridLayout* gameGridLayout;
    QWidget* gameGridWidget;
    QPushButton* gameButtons[3][3];
    QToolBar* toolBar;
    QAction* newGameAction;
    QAction* historyAction;
    QAction* logoutAction;

    // Game Logic
    Board* board;
    AIPlayer* aiPlayer;
    QString currentPlayer;
    QString player1Name;
    QString player2Name;
    QString gameMode;
    bool gameActive;
    QStringList moveHistory;
    QTimer* aiTimer;
};

#endif // MAINWINDOW_H
