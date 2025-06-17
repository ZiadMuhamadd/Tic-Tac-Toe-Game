#include "ResultDialog.h"

ResultDialog::ResultDialog(const QString& message, QWidget* parent)
    : QDialog(parent)
{
    setupUI(message);
    applyStyles();
    setFixedSize(350, 200);
    setWindowTitle("Game Result");
    setModal(true);
}

void ResultDialog::setupUI(const QString& message)
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(30, 30, 30, 30);

    // Message label
    messageLabel = new QLabel(message, this);
    messageLabel->setAlignment(Qt::AlignCenter);
    messageLabel->setObjectName("messageLabel");

    // Buttons layout
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(15);

    newGameButton = new QPushButton("New Game", this);
    newGameButton->setObjectName("actionButton");
    connect(newGameButton, &QPushButton::clicked, this, &ResultDialog::onNewGameClicked);

    mainMenuButton = new QPushButton("Main Menu", this);
    mainMenuButton->setObjectName("actionButton");
    connect(mainMenuButton, &QPushButton::clicked, this, &ResultDialog::onMainMenuClicked);

    buttonLayout->addWidget(newGameButton);
    buttonLayout->addWidget(mainMenuButton);

    mainLayout->addWidget(messageLabel);
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);
}

void ResultDialog::applyStyles()
{
    setStyleSheet(
        "ResultDialog {"
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
        "                                stop:0 #2c3e50, stop:1 #34495e);"
        "    border-radius: 10px;"
        "    border: 2px solid #3498db;"
        "}"
        "#messageLabel {"
        "    color: white;"
        "    font-size: 18px;"
        "    font-weight: bold;"
        "    padding: 20px;"
        "    background: transparent;"
        "}"
        "#actionButton {"
        "    color: white;"
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
        "                                stop:0 #3498db, stop:1 #2980b9);"
        "    border: none;"
        "    border-radius: 5px;"
        "    padding: 10px 20px;"
        "    font-size: 14px;"
        "    font-weight: bold;"
        "    min-width: 100px;"
        "}"
        "#actionButton:hover {"
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
        "                                stop:0 #27ae60, stop:1 #2ecc71);"
        "}"
        "#actionButton:pressed {"
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
        "                                stop:0 #1e8449, stop:1 #27ae60);"
        "}"
        );
}

void ResultDialog::onNewGameClicked()
{
    emit newGameRequested();
    accept();
}

void ResultDialog::onMainMenuClicked()
{
    emit mainMenuRequested();
    accept();
}
