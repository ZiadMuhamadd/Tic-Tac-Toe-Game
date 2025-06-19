#include "LoginDialog.h"
#include <QApplication>
#include <QScreen>

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent), currentStep(0)
{
    setWindowTitle("Tic Tac Toe - Login");

    // Make the dialog full screen using Qt 6 approach
    setWindowState(Qt::WindowFullScreen);
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    // Get screen size using Qt 6 method
    QScreen *screen = QApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    setGeometry(screenGeometry);

    setModal(true);

    setupUI();
    setBackgroundImage();
    setupStyling();
    resetToModeSelection();
}

void LoginDialog::setupUI()
{
    mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(20);  // Reduced spacing to fit more content
    mainLayout->setContentsMargins(60, 40, 60, 40);  // Reduced margins

    // Title - larger for full screen
    titleLabel = new QLabel("🎮 TIC TAC TOE 🎮", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setObjectName("titleLabel");

    // Instruction
    instructionLabel = new QLabel("Welcome! Choose your game mode:", this);
    instructionLabel->setAlignment(Qt::AlignCenter);
    instructionLabel->setObjectName("instructionLabel");

    // Mode selection
    modeComboBox = new QComboBox(this);
    modeComboBox->addItem("🎯 Player vs Player");
    modeComboBox->addItem("🤖 Player vs AI");
    modeComboBox->setObjectName("modeComboBox");

    continueButton = new QPushButton("Continue", this);
    continueButton->setObjectName("primaryButton");

    // Player info
    playerLabel = new QLabel(this);
    playerLabel->setAlignment(Qt::AlignCenter);
    playerLabel->setObjectName("playerLabel");

    // Authentication fields
    usernameLabel = new QLabel("Username:", this);
    usernameLabel->setObjectName("fieldLabel");

    usernameLineEdit = new QLineEdit(this);
    usernameLineEdit->setObjectName("inputField");
    usernameLineEdit->setPlaceholderText("Enter your username");

    passwordLabel = new QLabel("Password:", this);
    passwordLabel->setObjectName("fieldLabel");

    passwordLineEdit = new QLineEdit(this);
    passwordLineEdit->setObjectName("inputField");
    passwordLineEdit->setEchoMode(QLineEdit::Password);
    passwordLineEdit->setPlaceholderText("Enter your password");

    confirmPasswordLabel = new QLabel("Confirm Password:", this);
    confirmPasswordLabel->setObjectName("fieldLabel");

    confirmPasswordLineEdit = new QLineEdit(this);
    confirmPasswordLineEdit->setObjectName("inputField");
    confirmPasswordLineEdit->setEchoMode(QLineEdit::Password);
    confirmPasswordLineEdit->setPlaceholderText("Confirm your password");

    // Buttons
    signInButton = new QPushButton("Sign In", this);
    signInButton->setObjectName("primaryButton");

    newPlayerButton = new QPushButton("New Player", this);
    newPlayerButton->setObjectName("secondaryButton");

    nextPlayerButton = new QPushButton("Next Player", this);
    nextPlayerButton->setObjectName("primaryButton");

    backButton = new QPushButton("← Back", this);
    backButton->setObjectName("backButton");

    // Add Exit button for full screen mode
    QPushButton* exitButton = new QPushButton("✕ Exit", this);
    exitButton->setObjectName("exitButton");
    connect(exitButton, &QPushButton::clicked, this, &QDialog::reject);

    // Game start
    gameInfoLabel = new QLabel(this);
    gameInfoLabel->setAlignment(Qt::AlignCenter);
    gameInfoLabel->setObjectName("gameInfoLabel");

    startGameButton = new QPushButton("🚀 START GAME", this);
    startGameButton->setObjectName("startButton");

    // Create a centered container widget with increased width for full screen
    QWidget* centerWidget = new QWidget(this);
    centerWidget->setMaximumWidth(800);  // Increased from 600 to 800
    centerWidget->setMinimumWidth(600);  // Added minimum width
    centerWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    QVBoxLayout* centerLayout = new QVBoxLayout(centerWidget);
    centerLayout->setSpacing(15);  // Reduced spacing to fit more content
    centerLayout->setContentsMargins(20, 20, 20, 20);  // Added margins to center widget

    // Add widgets to center layout
    centerLayout->addWidget(titleLabel);
    centerLayout->addWidget(instructionLabel);
    centerLayout->addWidget(modeComboBox);
    centerLayout->addWidget(continueButton);
    centerLayout->addWidget(playerLabel);
    centerLayout->addWidget(usernameLabel);
    centerLayout->addWidget(usernameLineEdit);
    centerLayout->addWidget(passwordLabel);
    centerLayout->addWidget(passwordLineEdit);
    centerLayout->addWidget(confirmPasswordLabel);
    centerLayout->addWidget(confirmPasswordLineEdit);

    // Button layout with better spacing
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(15);  // Added spacing between buttons
    buttonLayout->addWidget(backButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(signInButton);
    buttonLayout->addWidget(newPlayerButton);
    buttonLayout->addWidget(nextPlayerButton);
    centerLayout->addLayout(buttonLayout);

    // Add some space before game info
    centerLayout->addSpacing(20);
    centerLayout->addWidget(gameInfoLabel);
    centerLayout->addWidget(startGameButton);

    // Create top-right exit button layout
    QHBoxLayout* topLayout = new QHBoxLayout();
    topLayout->setContentsMargins(0, 10, 10, 0);  // Added margins
    topLayout->addStretch();
    topLayout->addWidget(exitButton);

    // Add everything to main layout with better structure
    mainLayout->addLayout(topLayout);

    // Add flexible space at top
    mainLayout->addSpacing(20);

    // Center the main content horizontally
    QHBoxLayout* horizontalCenterLayout = new QHBoxLayout();
    horizontalCenterLayout->addStretch();
    horizontalCenterLayout->addWidget(centerWidget);
    horizontalCenterLayout->addStretch();

    mainLayout->addLayout(horizontalCenterLayout);

    // Add flexible space at bottom to ensure buttons are visible
    mainLayout->addSpacing(40);

    // Connect signals
    connect(continueButton, &QPushButton::clicked, this, &LoginDialog::onModeChanged);
    connect(signInButton, &QPushButton::clicked, this, &LoginDialog::onSignInClicked);
    connect(newPlayerButton, &QPushButton::clicked, this, &LoginDialog::onNewPlayerClicked);
    connect(nextPlayerButton, &QPushButton::clicked, this, &LoginDialog::onNextPlayerClicked);
    connect(startGameButton, &QPushButton::clicked, this, &LoginDialog::onStartGameClicked);
    connect(backButton, &QPushButton::clicked, this, &LoginDialog::onBackClicked);
}


void LoginDialog::setupStyling()
{
    QString style = R"(
        QDialog {
            background-color: rgba(0, 0, 0, 0.1);
        }

        #titleLabel {
            color: #FFD700;
            font-size: 42px;  /* Slightly reduced */
            font-weight: bold;
            text-shadow: 0 0 30px #FFD700, 0 0 40px #FFD700;
            background: rgba(25, 25, 112, 0.8);
            border: 4px solid #FFD700;
            border-radius: 25px;
            padding: 25px;  /* Reduced padding */
            margin: 15px;   /* Reduced margin */
        }

        #instructionLabel, #playerLabel, #gameInfoLabel {
            color: white;
            font-size: 20px;  /* Slightly reduced */
            font-weight: bold;
            text-shadow: 2px 2px 4px rgba(0,0,0,0.9);
            background: rgba(75, 0, 130, 0.7);
            border-radius: 15px;
            padding: 15px;  /* Reduced padding */
            border: 3px solid #9370DB;
            margin: 5px 0;  /* Added margin */
        }

        #fieldLabel {
            color: #E6E6FA;
            font-size: 16px;  /* Slightly reduced */
            font-weight: bold;
            text-shadow: 1px 1px 2px rgba(0,0,0,0.8);
            margin: 5px 0;
        }

        #inputField {
            background: rgba(25, 25, 112, 0.9);
            color: white;
            border: 3px solid #4169E1;
            border-radius: 12px;
            padding: 15px 18px;  /* Slightly reduced padding */
            font-size: 15px;     /* Slightly reduced font */
            selection-background-color: rgba(138, 43, 226, 0.7);
            min-height: 18px;    /* Reduced min-height */
            margin: 3px 0;       /* Added margin */
        }

        #inputField:focus {
            border: 3px solid #00BFFF;
            background: rgba(25, 25, 112, 1.0);
        }

        #modeComboBox {
            background: rgba(75, 0, 130, 0.9);
            color: white;
            border: 3px solid #9370DB;
            border-radius: 15px;
            padding: 15px 18px;  /* Slightly reduced padding */
            font-size: 16px;     /* Slightly reduced font */
            font-weight: bold;
            min-height: 22px;    /* Reduced min-height */
            margin: 5px 0;       /* Added margin */
        }

        #primaryButton {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 rgba(138, 43, 226, 0.9),
                stop:1 rgba(75, 0, 130, 0.9));
            color: white;
            border: 3px solid #FFD700;
            border-radius: 18px;
            font-size: 16px;     /* Slightly reduced font */
            font-weight: bold;
            padding: 15px 30px;  /* Slightly reduced padding */
            text-shadow: 1px 1px 2px rgba(0,0,0,0.8);
            min-height: 20px;    /* Reduced min-height */
            min-width: 120px;    /* Added min-width */
        }

        #primaryButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 rgba(138, 43, 226, 1.0),
                stop:1 rgba(75, 0, 130, 1.0));
            border: 3px solid #FFA500;
        }

        #secondaryButton {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 rgba(72, 61, 139, 0.9),
                stop:1 rgba(106, 90, 205, 0.9));
            color: white;
            border: 3px solid #9370DB;
            border-radius: 18px;
            font-size: 16px;     /* Slightly reduced font */
            font-weight: bold;
            padding: 15px 30px;  /* Slightly reduced padding */
            text-shadow: 1px 1px 2px rgba(0,0,0,0.8);
            min-height: 20px;    /* Reduced min-height */
            min-width: 120px;    /* Added min-width */
        }

        #secondaryButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 rgba(106, 90, 205, 1.0),
                stop:1 rgba(72, 61, 139, 1.0));
            border: 3px solid #BA55D3;
        }

        #backButton {
            background: rgba(220, 20, 60, 0.8);
            color: white;
            border: 3px solid #DC143C;
            border-radius: 15px;
            font-size: 14px;     /* Slightly reduced font */
            font-weight: bold;
            padding: 12px 20px;  /* Slightly reduced padding */
            min-width: 80px;     /* Added min-width */
        }

        #backButton:hover {
            background: rgba(220, 20, 60, 1.0);
            border: 3px solid #FF6347;
        }

        #exitButton {
            background: rgba(139, 0, 0, 0.8);
            color: white;
            border: 3px solid #8B0000;
            border-radius: 15px;
            font-size: 16px;
            font-weight: bold;
            padding: 12px 20px;  /* Slightly reduced padding */
            margin: 10px;        /* Reduced margin */
        }

        #exitButton:hover {
            background: rgba(139, 0, 0, 1.0);
            border: 3px solid #FF0000;
        }

        #startButton {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                stop:0 rgba(255, 215, 0, 0.9),
                stop:0.5 rgba(255, 140, 0, 0.9),
                stop:1 rgba(255, 69, 0, 0.9));
            color: #8B0000;
            border: 4px solid #FFD700;
            border-radius: 25px;
            font-size: 22px;     /* Slightly reduced font */
            font-weight: bold;
            padding: 20px 40px;  /* Slightly reduced padding */
            text-shadow: 1px 1px 2px rgba(255,255,255,0.8);
            margin: 10px 0;      /* Added margin */
        }

        #startButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                stop:0 rgba(255, 215, 0, 1.0),
                stop:0.5 rgba(255, 140, 0, 1.0),
                stop:1 rgba(255, 69, 0, 1.0));
            border: 4px solid #FFA500;
        }
    )";

    this->setStyleSheet(style);
}


void LoginDialog::setBackgroundImage()
{
    QPixmap loginBackground("D:/images/login_bg.jpg");

    if (!loginBackground.isNull()) {
        loginBackground = loginBackground.scaled(this->size(),
                                                 Qt::KeepAspectRatioByExpanding,
                                                 Qt::SmoothTransformation);

        QPalette palette;
        palette.setBrush(QPalette::Window, loginBackground);
        this->setPalette(palette);
        this->setAutoFillBackground(true);
    }
}

void LoginDialog::resizeEvent(QResizeEvent* event)
{
    QDialog::resizeEvent(event);
    setBackgroundImage();
}

void LoginDialog::resetToModeSelection()
{
    currentStep = 0;

    instructionLabel->setText("Welcome! Choose your game mode:");
    instructionLabel->show();
    modeComboBox->show();
    continueButton->show();

    playerLabel->hide();
    usernameLabel->hide();
    usernameLineEdit->hide();
    passwordLabel->hide();
    passwordLineEdit->hide();
    confirmPasswordLabel->hide();
    confirmPasswordLineEdit->hide();
    signInButton->hide();
    newPlayerButton->hide();
    nextPlayerButton->hide();
    backButton->hide();
    gameInfoLabel->hide();
    startGameButton->hide();

    player1Name.clear();
    player2Name.clear();
    usernameLineEdit->clear();
    passwordLineEdit->clear();
    confirmPasswordLineEdit->clear();
}

void LoginDialog::onModeChanged()
{
    gameMode = modeComboBox->currentIndex() == 0 ? "PvP" : "PvAI";
    showPlayer1Auth();
}

void LoginDialog::showPlayer1Auth()
{
    currentStep = 1;

    instructionLabel->hide();
    modeComboBox->hide();
    continueButton->hide();

    playerLabel->setText("🎮 Player 1 Authentication");
    playerLabel->show();
    usernameLabel->show();
    usernameLineEdit->show();
    passwordLabel->show();
    passwordLineEdit->show();
    signInButton->show();
    newPlayerButton->show();
    backButton->show();

    confirmPasswordLabel->hide();
    confirmPasswordLineEdit->hide();
    nextPlayerButton->hide();
    gameInfoLabel->hide();
    startGameButton->hide();
}

void LoginDialog::showPlayer2Auth()
{
    currentStep = 2;

    playerLabel->setText("🎮 Player 2 Authentication");
    usernameLineEdit->clear();
    passwordLineEdit->clear();
    confirmPasswordLineEdit->hide();
    confirmPasswordLabel->hide();
}

void LoginDialog::showGameStart()
{
    currentStep = 3;

    playerLabel->hide();
    usernameLabel->hide();
    usernameLineEdit->hide();
    passwordLabel->hide();
    passwordLineEdit->hide();
    signInButton->hide();
    newPlayerButton->hide();
    nextPlayerButton->hide();

    QString info = QString("🎯 Game Mode: %1\n👤 Player 1: %2")
                       .arg(gameMode == "PvP" ? "Player vs Player" : "Player vs AI")
                       .arg(player1Name);

    if (gameMode == "PvP") {
        info += QString("\n👤 Player 2: %1").arg(player2Name);
    }

    gameInfoLabel->setText(info);
    gameInfoLabel->show();
    startGameButton->show();
}

void LoginDialog::onSignInClicked()
{
    QString username = usernameLineEdit->text().trimmed();
    QString password = passwordLineEdit->text();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter both username and password.");
        return;
    }

    if (authenticateUser(username, password)) {
        if (currentStep == 1) {
            player1Name = username;
            if (gameMode == "PvP") {
                showPlayer2Auth();
            } else {
                player2Name = "AI";
                showGameStart();
            }
        } else if (currentStep == 2) {
            if (username == player1Name) {
                QMessageBox::warning(this, "Error", "Player 2 must be different from Player 1!");
                return;
            }
            player2Name = username;
            showGameStart();
        }
    } else {
        QMessageBox::warning(this, "Authentication Failed", "Invalid username or password.");
    }
}

void LoginDialog::onNewPlayerClicked()
{
    confirmPasswordLabel->show();
    confirmPasswordLineEdit->show();

    signInButton->setText("Register");
    newPlayerButton->hide();
    nextPlayerButton->show();
    nextPlayerButton->setText("Register");

    disconnect(signInButton, &QPushButton::clicked, this, &LoginDialog::onSignInClicked);
    connect(signInButton, &QPushButton::clicked, this, &LoginDialog::onNextPlayerClicked);
}

void LoginDialog::onNextPlayerClicked()
{
    QString username = usernameLineEdit->text().trimmed();
    QString password = passwordLineEdit->text();
    QString confirmPassword = confirmPasswordLineEdit->text();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter username and password.");
        return;
    }

    if (confirmPasswordLineEdit->isVisible() && password != confirmPassword) {
        QMessageBox::warning(this, "Password Error", "Passwords do not match.");
        return;
    }

    if (confirmPasswordLineEdit->isVisible()) {
        if (registerUser(username, password)) {
            QMessageBox::information(this, "Success", "User registered successfully!");

            confirmPasswordLabel->hide();
            confirmPasswordLineEdit->hide();
            signInButton->setText("Sign In");
            newPlayerButton->show();
            nextPlayerButton->hide();

            disconnect(signInButton, &QPushButton::clicked, this, &LoginDialog::onNextPlayerClicked);
            connect(signInButton, &QPushButton::clicked, this, &LoginDialog::onSignInClicked);

            usernameLineEdit->clear();
            passwordLineEdit->clear();
        } else {
            QMessageBox::warning(this, "Registration Failed", "Username already exists.");
        }
    }
}

void LoginDialog::onStartGameClicked()
{
    accept();
}

void LoginDialog::onBackClicked()
{
    resetToModeSelection();
}

bool LoginDialog::authenticateUser(const QString& username, const QString& password)
{
    QFile file("users.json");
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    QJsonObject users = doc.object();
    file.close();

    if (users.contains(username)) {
        QString storedHash = users[username].toString();
        QString inputHash = hashPassword(password);
        return storedHash == inputHash;
    }

    return false;
}

bool LoginDialog::registerUser(const QString& username, const QString& password)
{
    QFile file("users.json");
    QJsonObject users;

    if (file.open(QIODevice::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        users = doc.object();
        file.close();
    }

    if (users.contains(username)) {
        return false;
    }

    users[username] = hashPassword(password);

    if (file.open(QIODevice::WriteOnly)) {
        QJsonDocument doc(users);
        file.write(doc.toJson());
        file.close();
        return true;
    }

    return false;
}

QString LoginDialog::hashPassword(const QString& password)
{
    return QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex();
}
