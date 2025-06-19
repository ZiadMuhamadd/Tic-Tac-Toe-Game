#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QCryptographicHash>
#include <QPixmap>
#include <QPalette>
#include <QResizeEvent>

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog() = default;

    QString getPlayer1Name() const { return player1Name; }
    QString getPlayer2Name() const { return player2Name; }
    QString getGameMode() const { return gameMode; }

protected:
    void resizeEvent(QResizeEvent* event) override;

private slots:
    void onModeChanged();
    void onSignInClicked();
    void onNewPlayerClicked();
    void onNextPlayerClicked();
    void onStartGameClicked();
    void onBackClicked();

private:
    void setupUI();
    void setupStyling();
    void setBackgroundImage();
    void resetToModeSelection();
    void showPlayer1Auth();
    void showPlayer2Auth();
    void showGameStart();

    bool authenticateUser(const QString& username, const QString& password);
    bool registerUser(const QString& username, const QString& password);
    QString hashPassword(const QString& password);

    // UI Components
    QVBoxLayout* mainLayout;
    QLabel* titleLabel;
    QLabel* instructionLabel;
    QComboBox* modeComboBox;
    QPushButton* continueButton;
    QLabel* playerLabel;
    QLabel* usernameLabel;
    QLabel* passwordLabel;
    QLabel* confirmPasswordLabel;
    QLineEdit* usernameLineEdit;
    QLineEdit* passwordLineEdit;
    QLineEdit* confirmPasswordLineEdit;
    QPushButton* signInButton;
    QPushButton* newPlayerButton;
    QPushButton* nextPlayerButton;
    QPushButton* backButton;
    QLabel* gameInfoLabel;
    QPushButton* startGameButton;

    // Data
    QString player1Name;
    QString player2Name;
    QString gameMode;
    int currentStep;
};

#endif // LOGINDIALOG_H
