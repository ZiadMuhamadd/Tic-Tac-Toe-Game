#ifndef RESULTDIALOG_H
#define RESULTDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

class ResultDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ResultDialog(const QString& message, QWidget* parent = nullptr);

private slots:
    void onNewGameClicked();
    void onMainMenuClicked();

private:
    void setupUI(const QString& message);
    void applyStyles();

    QLabel* messageLabel;
    QPushButton* newGameButton;
    QPushButton* mainMenuButton;

signals:
    void newGameRequested();
    void mainMenuRequested();
};

#endif // RESULTDIALOG_H
