#include "MainWindow.h"
#include "LoginDialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), currentPlayer("X"), gameActive(true)
{
    setWindowTitle("Tic Tac Toe - Synthwave Edition");
    setMinimumSize(800, 900);
    resize(1000, 1100);

    board = new Board();
    aiPlayer = new AIPlayer();
    aiTimer = new QTimer(this);
    aiTimer->setSingleShot(true);

    setupUI();
    setBackgroundImage();
    setupStyling();
    setupToolbar();

    connect(aiTimer, &QTimer::timeout, this, &MainWindow::makeAIMove);
}
void MainWindow::setupUI()
{
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // Create main horizontal layout
    QHBoxLayout* mainHorizontalLayout = new QHBoxLayout(centralWidget);
    mainHorizontalLayout->setSpacing(20);
    mainHorizontalLayout->setContentsMargins(20, 20, 20, 20);

    // Create left panel for banners
    QWidget* leftPanel = new QWidget(this);
    leftPanel->setObjectName("leftPanel");
    leftPanel->setFixedWidth(350);  // Fixed width for left panel
    leftPanel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

    QVBoxLayout* leftLayout = new QVBoxLayout(leftPanel);
    leftLayout->setSpacing(15);
    leftLayout->setContentsMargins(10, 10, 10, 10);

    // Title (more square-shaped)
    titleLabel = new QLabel("🎮 TIC TAC TOE 🎮", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setObjectName("titleLabel");
    titleLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    // Players info (more square-shaped)
    playersLabel = new QLabel(this);
    playersLabel->setAlignment(Qt::AlignCenter);
    playersLabel->setObjectName("playersLabel");
    playersLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    // Status (more square-shaped)
    statusLabel = new QLabel("Player X's Turn", this);
    statusLabel->setAlignment(Qt::AlignCenter);
    statusLabel->setObjectName("statusLabel");
    statusLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    // Add banners to left panel
    leftLayout->addWidget(titleLabel);
    leftLayout->addWidget(playersLabel);
    leftLayout->addWidget(statusLabel);
    leftLayout->addStretch();  // Push banners to top

    // Create right panel for game grid
    QWidget* rightPanel = new QWidget(this);
    rightPanel->setObjectName("rightPanel");
    rightPanel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QVBoxLayout* rightLayout = new QVBoxLayout(rightPanel);
    rightLayout->setContentsMargins(20, 20, 20, 20);

    // Game grid
    setupGameGrid();

    // Center the grid vertically and horizontally in right panel
    QHBoxLayout* gridCenterLayout = new QHBoxLayout();
    gridCenterLayout->addStretch();
    gridCenterLayout->addWidget(gameGridWidget);
    gridCenterLayout->addStretch();

    rightLayout->addStretch();
    rightLayout->addLayout(gridCenterLayout);
    rightLayout->addStretch();

    // Add panels to main layout
    mainHorizontalLayout->addWidget(leftPanel);
    mainHorizontalLayout->addWidget(rightPanel);
}

void MainWindow::setupGameGrid()
{
    gameGridWidget = new QWidget(this);
    gameGridWidget->setObjectName("gameGridWidget");

    // Now we can make the grid much larger since we have more vertical space
    gameGridWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    gameGridWidget->setMinimumSize(500, 500);  // Larger minimum size
    gameGridWidget->setMaximumSize(800, 800);  // Much larger maximum size

    gameGridLayout = new QGridLayout(gameGridWidget);
    gameGridLayout->setSpacing(20);  // Increased spacing
    gameGridLayout->setContentsMargins(30, 30, 30, 30);

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            gameButtons[i][j] = new QPushButton("", this);
            gameButtons[i][j]->setObjectName("gameButton");

            // Can make buttons larger now with more space
            gameButtons[i][j]->setMinimumSize(140, 140);  // Larger buttons
            gameButtons[i][j]->setMaximumSize(200, 200);  // Larger maximum

            gameButtons[i][j]->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

            gameButtons[i][j]->setProperty("row", i);
            gameButtons[i][j]->setProperty("col", j);

            connect(gameButtons[i][j], &QPushButton::clicked, this, &MainWindow::onGameButtonClicked);

            gameGridLayout->addWidget(gameButtons[i][j], i, j);
        }
    }

    for (int i = 0; i < 3; ++i) {
        gameGridLayout->setRowStretch(i, 1);
        gameGridLayout->setColumnStretch(i, 1);
    }
}

void MainWindow::setupStyling()
{
    QString style = R"(
        QMainWindow {
            background-color: rgba(0, 0, 0, 0.1);
        }

        #leftPanel {
            background: rgba(0, 0, 0, 0.3);
            border: 2px solid #8A2BE2;
            border-radius: 15px;
        }

        #rightPanel {
            background: rgba(0, 0, 0, 0.1);
        }

        #titleLabel {
            color: #00FFFF;
            font-size: 20px;  /* Adjusted for square format */
            font-weight: bold;
            text-shadow: 0 0 15px #00FFFF, 0 0 20px #00FFFF;
            background: rgba(0, 0, 0, 0.8);
            border: 2px solid #00FFFF;
            border-radius: 15px;
            padding: 20px;  /* More square padding */
            margin: 5px;
            min-height: 60px;  /* Square-ish height */
        }

        #playersLabel {
            color: #FF1493;
            font-size: 16px;  /* Adjusted for square format */
            font-weight: bold;
            text-shadow: 0 0 8px #FF1493;
            background: rgba(0, 0, 0, 0.7);
            border: 2px solid #FF1493;
            border-radius: 15px;
            padding: 20px;  /* More square padding */
            margin: 5px;
            min-height: 60px;  /* Square-ish height */
        }

        #statusLabel {
            color: #00FFFF;
            font-size: 18px;  /* Adjusted for square format */
            font-weight: bold;
            text-shadow: 0 0 10px #00FFFF;
            background: rgba(0, 0, 0, 0.8);
            border: 2px solid #00FFFF;
            border-radius: 15px;
            padding: 20px;  /* More square padding */
            margin: 5px;
            min-height: 60px;  /* Square-ish height */
        }

        #gameGridWidget {
            background: rgba(0, 0, 0, 0.6);
            border: 4px solid #8A2BE2;  /* Thicker border for prominence */
            border-radius: 25px;
        }

        #gameButton {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                stop:0 rgba(0, 0, 0, 0.8),
                stop:1 rgba(75, 0, 130, 0.6));
            border: 3px solid #00FFFF;
            border-radius: 15px;
            color: white;
            font-size: 56px;  /* Larger font for bigger buttons */
            font-weight: bold;
            text-shadow: 0 0 15px currentColor;
        }

        #gameButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                stop:0 rgba(255, 20, 147, 0.4),
                stop:1 rgba(138, 43, 226, 0.4));
            border: 4px solid #FF1493;  /* Thicker hover border */
            text-shadow: 0 0 25px currentColor;
        }

        #gameButton:pressed {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                stop:0 rgba(138, 43, 226, 0.8),
                stop:1 rgba(75, 0, 130, 0.8));
            border: 3px solid #8A2BE2;
        }

        QToolBar {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 rgba(138, 43, 226, 0.9),
                stop:0.5 rgba(255, 20, 147, 0.9),
                stop:1 rgba(138, 43, 226, 0.9));
            border: none;
            spacing: 15px;
            padding: 8px;
        }

        QToolButton {
            background: rgba(0, 0, 0, 0.7);
            color: white;
            border: 2px solid #00FFFF;
            border-radius: 8px;
            padding: 8px 15px;
            font-weight: bold;
            font-size: 12px;
            text-shadow: 0 0 5px rgba(255, 255, 255, 0.8);
        }

        QToolButton:hover {
            background: rgba(255, 20, 147, 0.8);
            border: 2px solid #FF1493;
            text-shadow: 0 0 10px #FF1493;
        }

        QToolButton:pressed {
            background: rgba(138, 43, 226, 0.8);
            border: 2px solid #8A2BE2;
        }

        QStatusBar {
            background: rgba(0, 0, 0, 0.8);
            color: #00FFFF;
            border-top: 2px solid #00FFFF;
        }
    )";

    this->setStyleSheet(style);
}



void MainWindow::setBackgroundImage()
{
    QPixmap gameBackground("D:/images/game_bg.jpg"); // Change this path to your image location

    if (!gameBackground.isNull()) {
        gameBackground = gameBackground.scaled(this->size(),
                                               Qt::KeepAspectRatioByExpanding,
                                               Qt::SmoothTransformation);

        QPalette palette;
        palette.setBrush(QPalette::Window, gameBackground);
        this->setPalette(palette);
        this->setAutoFillBackground(true);
    }
}



void MainWindow::setupToolbar()
{
    toolBar = addToolBar("Game Controls");
    toolBar->setMovable(false);
    toolBar->setFloatable(false);

    newGameAction = new QAction("🎯 New Game", this);
    historyAction = new QAction("📊 History", this);
    logoutAction = new QAction("🚪 Logout", this);

    toolBar->addAction(newGameAction);
    toolBar->addSeparator();
    toolBar->addAction(historyAction);
    toolBar->addSeparator();
    toolBar->addAction(logoutAction);

    connect(newGameAction, &QAction::triggered, this, &MainWindow::onNewGameClicked);
    connect(historyAction, &QAction::triggered, this, &MainWindow::onShowHistoryClicked);
    connect(logoutAction, &QAction::triggered, this, &MainWindow::onLogoutClicked);
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event);
    setBackgroundImage();
}

void MainWindow::setPlayers(const QString& p1, const QString& p2, const QString& mode)
{
    player1Name = p1;
    player2Name = p2;
    gameMode = mode;

    QString playersText = QString("👤 %1 (X) vs %2 (O)")
                              .arg(player1Name)
                              .arg(player2Name);

    if (gameMode == "PvAI") {
        playersText = QString("👤 %1 (X) vs 🤖 AI (O)").arg(player1Name);
    }

    playersLabel->setText(playersText);
    updateGameStatus();
}

void MainWindow::onGameButtonClicked()
{
    if (!gameActive) return;

    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (!button || !button->text().isEmpty()) return;

    int row = button->property("row").toInt();
    int col = button->property("col").toInt();

    if (board->makeMove(row, col, currentPlayer.at(0).toLatin1())) {
        button->setText(currentPlayer);

        // Apply color styling based on player
        if (currentPlayer == "X") {
            button->setStyleSheet(button->styleSheet() + " color: #FF1493; text-shadow: 0 0 15px #FF1493;");
        } else {
            button->setStyleSheet(button->styleSheet() + " color: #00FFFF; text-shadow: 0 0 15px #00FFFF;");
        }

        animateButton(button);

        QString move = QString("%1%2%3").arg(currentPlayer).arg(row).arg(col);
        moveHistory.append(move);

        checkGameEnd();

        if (gameActive) {
            currentPlayer = (currentPlayer == "X") ? "O" : "X";
            updateGameStatus();

            // AI move if needed
            if (gameMode == "PvAI" && currentPlayer == "O") {
                statusLabel->setText("🤖 AI is thinking...");
                aiTimer->start(1000); // 1 second delay for AI move
            }
        }
    }
}

void MainWindow::makeAIMove()
{
    if (!gameActive || currentPlayer != "O") return;

    auto availableMoves = board->getAvailableMoves();
    if (!availableMoves.empty()) {
        auto move = aiPlayer->getMove(board);
        int row = move.first;
        int col = move.second;

        if (board->makeMove(row, col, 'O')) {
            gameButtons[row][col]->setText("O");
            gameButtons[row][col]->setStyleSheet(gameButtons[row][col]->styleSheet() +
                                                 " color: #00FFFF; text-shadow: 0 0 15px #00FFFF;");
            animateButton(gameButtons[row][col]);

            QString moveStr = QString("O%1%2").arg(row).arg(col);
            moveHistory.append(moveStr);

            checkGameEnd();

            if (gameActive) {
                currentPlayer = "X";
                updateGameStatus();
            }
        }
    }
}

// Fixed animateButton function implementation
void MainWindow::animateButton(QPushButton* button)
{
    if (!button) return;

    // Create a simple scale animation
    QPropertyAnimation* animation = new QPropertyAnimation(button, "geometry");
    animation->setDuration(150);

    QRect originalGeometry = button->geometry();
    QRect scaledGeometry = originalGeometry.adjusted(-3, -3, 3, 3);

    animation->setStartValue(originalGeometry);
    animation->setEndValue(scaledGeometry);
    animation->setEasingCurve(QEasingCurve::OutBounce);

    // Create return animation
    QPropertyAnimation* returnAnimation = new QPropertyAnimation(button, "geometry");
    returnAnimation->setDuration(150);
    returnAnimation->setStartValue(scaledGeometry);
    returnAnimation->setEndValue(originalGeometry);
    returnAnimation->setEasingCurve(QEasingCurve::InBounce);

    // Connect animations
    connect(animation, &QPropertyAnimation::finished, [returnAnimation]() {
        returnAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    });

    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void MainWindow::updateGameStatus()
{
    if (!gameActive) return;

    if (currentPlayer == "X") {
        statusLabel->setText(QString("🎯 %1's Turn (X)").arg(player1Name));
    } else {
        if (gameMode == "PvAI") {
            statusLabel->setText("🤖 AI's Turn (O)");
        } else {
            statusLabel->setText(QString("🎯 %1's Turn (O)").arg(player2Name));
        }
    }
}

void MainWindow::checkGameEnd()
{
    char winner = '\0';

    if (board->checkWin('X')) {
        winner = 'X';
    } else if (board->checkWin('O')) {
        winner = 'O';
    } else if (board->checkTie()) {
        winner = 'T'; // Tie
    }

    if (winner != '\0') {
        gameActive = false;

        QString result;
        if (winner == 'T') {
            result = "It's a Tie! 🤝";
            statusLabel->setText("🤝 Game Tied!");
        } else if (winner == 'X') {
            result = QString("%1 Wins! 🎉").arg(player1Name);
            statusLabel->setText(QString("🎉 %1 Wins!").arg(player1Name));
        } else {
            if (gameMode == "PvAI") {
                result = "AI Wins! 🤖";
                statusLabel->setText("🤖 AI Wins!");
            } else {
                result = QString("%1 Wins! 🎉").arg(player2Name);
                statusLabel->setText(QString("🎉 %1 Wins!").arg(player2Name));
            }
        }

        saveGameHistory(QString(winner));

        QTimer::singleShot(1500, [this, result]() {
            showGameOverDialog(result);
        });
    }
}

void MainWindow::showGameOverDialog(const QString& result)
{
    QDialog* gameOverDialog = new QDialog(this);
    gameOverDialog->setWindowTitle("Game Over");
    gameOverDialog->setFixedSize(450, 350);
    gameOverDialog->setModal(true);

    // Set XOXO background
    QPixmap xoxoBackground("glowing-xoxo-neon-typography-dark-purple-background.jpg"); // Change path
    if (!xoxoBackground.isNull()) {
        xoxoBackground = xoxoBackground.scaled(gameOverDialog->size(),
                                               Qt::KeepAspectRatioByExpanding,
                                               Qt::SmoothTransformation);

        QPalette palette;
        palette.setBrush(QPalette::Window, xoxoBackground);
        gameOverDialog->setPalette(palette);
        gameOverDialog->setAutoFillBackground(true);
    }

    QVBoxLayout* layout = new QVBoxLayout(gameOverDialog);
    layout->setSpacing(30);
    layout->setContentsMargins(40, 40, 40, 40);

    QLabel* resultLabel = new QLabel(result, gameOverDialog);
    resultLabel->setAlignment(Qt::AlignCenter);
    resultLabel->setStyleSheet(R"(
        QLabel {
            color: white;
            font-size: 28px;
            font-weight: bold;
            text-shadow: 0 0 20px #FF1493, 0 0 30px #FF1493;
            background: rgba(0, 0, 0, 0.7);
            border: 3px solid #FF1493;
            border-radius: 20px;
            padding: 25px;
        }
    )");

    QHBoxLayout* buttonLayout = new QHBoxLayout();

    QPushButton* newGameButton = new QPushButton("🎯 New Game", gameOverDialog);
    QPushButton* closeButton = new QPushButton("🚪 Close", gameOverDialog);

    QString dialogButtonStyle = R"(
        QPushButton {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                stop:0 rgba(255, 20, 147, 0.9),
                stop:1 rgba(138, 43, 226, 0.9));
            color: white;
            border: 2px solid #FF1493;
            border-radius: 15px;
            font-size: 16px;
            font-weight: bold;
            padding: 15px 25px;
            text-shadow: 0 0 10px rgba(255, 255, 255, 0.8);
        }
        QPushButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                stop:0 rgba(138, 43, 226, 1.0),
                stop:1 rgba(255, 20, 147, 1.0));
            border: 2px solid #00FFFF;
            text-shadow: 0 0 15px #00FFFF;
        }
        QPushButton:pressed {
            background: rgba(75, 0, 130, 0.9);
            border: 2px solid #8A2BE2;
        }
    )";

    newGameButton->setStyleSheet(dialogButtonStyle);
    closeButton->setStyleSheet(dialogButtonStyle);

    buttonLayout->addWidget(newGameButton);
    buttonLayout->addWidget(closeButton);

    layout->addWidget(resultLabel);
    layout->addLayout(buttonLayout);

    connect(newGameButton, &QPushButton::clicked, [this, gameOverDialog]() {
        gameOverDialog->accept();
        resetGame();
    });

    connect(closeButton, &QPushButton::clicked, gameOverDialog, &QDialog::accept);

    gameOverDialog->exec();
    delete gameOverDialog;
}

void MainWindow::saveGameHistory(const QString& winner)
{
    QFile file("game_history.json");
    QJsonObject history;

    if (file.open(QIODevice::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        history = doc.object();
        file.close();
    }

    QJsonObject gameData;
    gameData["winner"] = winner;
    gameData["mode"] = gameMode;
    gameData["opponent"] = player2Name;

    QJsonArray movesArray;
    for (const QString& move : moveHistory) {
        movesArray.append(move);
    }
    gameData["moves"] = movesArray;

    QJsonArray userGames;
    if (history.contains(player1Name)) {
        userGames = history[player1Name].toArray();
    }
    userGames.append(gameData);
    history[player1Name] = userGames;

    if (file.open(QIODevice::WriteOnly)) {
        QJsonDocument doc(history);
        file.write(doc.toJson());
        file.close();
    }
}

void MainWindow::onNewGameClicked()
{
    resetGame();
}

void MainWindow::onShowHistoryClicked()
{
    QMessageBox::information(this, "History", "Game history feature coming soon!");
}

void MainWindow::onLogoutClicked()
{
    int ret = QMessageBox::question(this, "Logout",
                                    "Are you sure you want to logout?",
                                    QMessageBox::Yes | QMessageBox::No);

    if (ret == QMessageBox::Yes) {
        this->close();

        LoginDialog* loginDialog = new LoginDialog();
        if (loginDialog->exec() == QDialog::Accepted) {
            setPlayers(loginDialog->getPlayer1Name(),
                       loginDialog->getPlayer2Name(),
                       loginDialog->getGameMode());
            resetGame();
        } else {
            QApplication::quit();
        }
        delete loginDialog;
    }
}

void MainWindow::resetGame()
{
    board->reset();
    gameActive = true;
    currentPlayer = "X";
    moveHistory.clear();

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            gameButtons[i][j]->setText("");
            gameButtons[i][j]->setStyleSheet(""); // Reset styling
        }
    }

    updateGameStatus();
}
