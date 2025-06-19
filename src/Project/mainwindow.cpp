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
    QPixmap xoxoBackground("D:/images/gameover_bg.jpg"); // Change path
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
    showGameHistoryDialog();
}

void MainWindow::showGameHistoryDialog()
{
    QDialog* historyDialog = new QDialog(this);
    historyDialog->setWindowTitle("Game History");
    historyDialog->setFixedSize(800, 600);
    historyDialog->setModal(true);

    // Set background for history dialog
    QPixmap historyBackground("D:/images/gameover_bg.jpg");
    if (!historyBackground.isNull()) {
        historyBackground = historyBackground.scaled(historyDialog->size(),
                                                     Qt::KeepAspectRatioByExpanding,
                                                     Qt::SmoothTransformation);

        QPalette palette;
        palette.setBrush(QPalette::Window, historyBackground);
        historyDialog->setPalette(palette);
        historyDialog->setAutoFillBackground(true);
    }

    QVBoxLayout* mainLayout = new QVBoxLayout(historyDialog);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    // Title
    QLabel* titleLabel = new QLabel(QString("📊 %1's Game History").arg(player1Name), historyDialog);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet(R"(
        QLabel {
            color: #00FFFF;
            font-size: 24px;
            font-weight: bold;
            text-shadow: 0 0 15px #00FFFF;
            background: rgba(0, 0, 0, 0.8);
            border: 2px solid #00FFFF;
            border-radius: 15px;
            padding: 15px;
            margin: 10px;
        }
    )");

    // Games list
    QListWidget* gamesList = new QListWidget(historyDialog);
    gamesList->setStyleSheet(R"(
        QListWidget {
            background: rgba(0, 0, 0, 0.8);
            color: white;
            border: 2px solid #8A2BE2;
            border-radius: 10px;
            font-size: 14px;
            padding: 10px;
        }
        QListWidget::item {
            background: rgba(75, 0, 130, 0.6);
            border: 1px solid #9370DB;
            border-radius: 8px;
            padding: 10px;
            margin: 3px;
        }
        QListWidget::item:selected {
            background: rgba(138, 43, 226, 0.8);
            border: 2px solid #FF1493;
        }
        QListWidget::item:hover {
            background: rgba(106, 90, 205, 0.7);
            border: 1px solid #BA55D3;
        }
    )");

    // Load game history
    QFile file("D:/images/gameover_bg.jpg");
    QJsonArray userGames;

    if (file.open(QIODevice::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        QJsonObject history = doc.object();
        file.close();

        if (history.contains(player1Name)) {
            userGames = history[player1Name].toArray();
        }
    }

    // Populate games list
    for (int i = 0; i < userGames.size(); ++i) {
        QJsonObject gameData = userGames[i].toObject();
        QString winner = gameData["winner"].toString();
        QString mode = gameData["mode"].toString();
        QString opponent = gameData["opponent"].toString();
        QJsonArray moves = gameData["moves"].toArray();

        QString resultText;
        if (winner == "T") {
            resultText = "🤝 Tie";
        } else if (winner == "X") {
            resultText = QString("🎉 %1 Won").arg(player1Name);
        } else {
            if (mode == "PvAI") {
                resultText = "🤖 AI Won";
            } else {
                resultText = QString("🎉 %1 Won").arg(opponent);
            }
        }

        QString modeText = (mode == "PvP") ? "Player vs Player" : "Player vs AI";
        QString itemText = QString("Game #%1 | %2 | %3 vs %4 | %5 moves | %6")
                               .arg(i + 1)
                               .arg(resultText)
                               .arg(player1Name)
                               .arg(opponent)
                               .arg(moves.size())
                               .arg(modeText);

        QListWidgetItem* item = new QListWidgetItem(itemText);
        item->setData(Qt::UserRole, gameData);
        gamesList->addItem(item);
    }

    // Buttons layout
    QHBoxLayout* buttonLayout = new QHBoxLayout();

    QPushButton* replayButton = new QPushButton("🎬 Replay Game", historyDialog);
    QPushButton* deleteButton = new QPushButton("🗑️ Delete Game", historyDialog);
    QPushButton* clearAllButton = new QPushButton("🧹 Clear All", historyDialog);
    QPushButton* closeButton = new QPushButton("✕ Close", historyDialog);

    QString buttonStyle = R"(
        QPushButton {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                stop:0 rgba(138, 43, 226, 0.9),
                stop:1 rgba(255, 20, 147, 0.9));
            color: white;
            border: 2px solid #FF1493;
            border-radius: 12px;
            font-size: 14px;
            font-weight: bold;
            padding: 10px 20px;
            text-shadow: 0 0 8px rgba(255, 255, 255, 0.8);
        }
        QPushButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                stop:0 rgba(255, 20, 147, 1.0),
                stop:1 rgba(138, 43, 226, 1.0));
            border: 2px solid #00FFFF;
            text-shadow: 0 0 12px #00FFFF;
        }
        QPushButton:pressed {
            background: rgba(75, 0, 130, 0.9);
            border: 2px solid #8A2BE2;
        }
        QPushButton:disabled {
            background: rgba(64, 64, 64, 0.5);
            color: rgba(255, 255, 255, 0.5);
            border: 2px solid rgba(128, 128, 128, 0.5);
        }
    )";

    replayButton->setStyleSheet(buttonStyle);
    deleteButton->setStyleSheet(buttonStyle);
    clearAllButton->setStyleSheet(buttonStyle);
    closeButton->setStyleSheet(buttonStyle);

    // Initially disable buttons
    replayButton->setEnabled(false);
    deleteButton->setEnabled(false);
    clearAllButton->setEnabled(userGames.size() > 0);

    buttonLayout->addWidget(replayButton);
    buttonLayout->addWidget(deleteButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(clearAllButton);
    buttonLayout->addWidget(closeButton);

    // Stats label
    QLabel* statsLabel = new QLabel(historyDialog);
    if (userGames.size() == 0) {
        statsLabel->setText("📈 No games played yet. Start playing to build your history!");
    } else {
        int wins = 0, losses = 0, ties = 0;
        for (int i = 0; i < userGames.size(); ++i) {
            QString winner = userGames[i].toObject()["winner"].toString();
            if (winner == "X") wins++;
            else if (winner == "O") losses++;
            else ties++;
        }

        statsLabel->setText(QString("📈 Stats: %1 Wins | %2 Losses | %3 Ties | Total: %4 games")
                                .arg(wins).arg(losses).arg(ties).arg(userGames.size()));
    }

    statsLabel->setStyleSheet(R"(
        QLabel {
            color: #FFD700;
            font-size: 16px;
            font-weight: bold;
            text-shadow: 0 0 10px #FFD700;
            background: rgba(0, 0, 0, 0.7);
            border: 2px solid #FFD700;
            border-radius: 10px;
            padding: 10px;
            margin: 5px;
        }
    )");

    // Add widgets to layout
    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(statsLabel);
    mainLayout->addWidget(gamesList, 1);
    mainLayout->addLayout(buttonLayout);

    // Connect signals
    connect(gamesList, &QListWidget::itemSelectionChanged, [=]() {
        bool hasSelection = gamesList->currentItem() != nullptr;
        replayButton->setEnabled(hasSelection);
        deleteButton->setEnabled(hasSelection);
    });

    connect(replayButton, &QPushButton::clicked, [=]() {
        QListWidgetItem* item = gamesList->currentItem();
        if (item) {
            QJsonObject gameData = item->data(Qt::UserRole).toJsonObject();
            historyDialog->accept();
            replayGame(gameData);
        }
    });

    connect(deleteButton, &QPushButton::clicked, [=]() {
        QListWidgetItem* item = gamesList->currentItem();
        if (item) {
            int ret = QMessageBox::question(historyDialog, "Delete Game",
                                            "Are you sure you want to delete this game?",
                                            QMessageBox::Yes | QMessageBox::No);
            if (ret == QMessageBox::Yes) {
                int row = gamesList->row(item);
                deleteGameFromHistory(row);
                historyDialog->accept();
                showGameHistoryDialog(); // Refresh the dialog
            }
        }
    });

    connect(clearAllButton, &QPushButton::clicked, [=]() {
        int ret = QMessageBox::question(historyDialog, "Clear All History",
                                        "Are you sure you want to delete ALL game history?",
                                        QMessageBox::Yes | QMessageBox::No);
        if (ret == QMessageBox::Yes) {
            // Clear all history for current user
            QFile file("game_history.json");
            QJsonObject history;

            if (file.open(QIODevice::ReadOnly)) {
                QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
                history = doc.object();
                file.close();
            }

            history.remove(player1Name);

            if (file.open(QIODevice::WriteOnly)) {
                QJsonDocument doc(history);
                file.write(doc.toJson());
                file.close();
            }

            historyDialog->accept();
            QMessageBox::information(this, "History Cleared", "All game history has been cleared!");
        }
    });

    connect(closeButton, &QPushButton::clicked, historyDialog, &QDialog::accept);

    historyDialog->exec();
    delete historyDialog;
}

void MainWindow::replayGame(const QJsonObject& gameData)
{
    // Reset the game board
    resetGame();

    QJsonArray moves = gameData["moves"].toArray();
    QString mode = gameData["mode"].toString();
    QString opponent = gameData["opponent"].toString();

    // Create replay dialog
    QDialog* replayDialog = new QDialog(this);
    replayDialog->setWindowTitle("Game Replay");
    replayDialog->setFixedSize(400, 300);
    replayDialog->setModal(true);

    // Set background
    QPixmap replayBackground("D:/images/gameover_bg.jpg");
    if (!replayBackground.isNull()) {
        replayBackground = replayBackground.scaled(replayDialog->size(),
                                                   Qt::KeepAspectRatioByExpanding,
                                                   Qt::SmoothTransformation);

        QPalette palette;
        palette.setBrush(QPalette::Window, replayBackground);
        replayDialog->setPalette(palette);
        replayDialog->setAutoFillBackground(true);
    }

    QVBoxLayout* layout = new QVBoxLayout(replayDialog);
    layout->setSpacing(20);
    layout->setContentsMargins(30, 30, 30, 30);

    QLabel* titleLabel = new QLabel("🎬 Game Replay", replayDialog);
    titleLabel->setAlignment(Qt::AlignCenter);

    QLabel* infoLabel = new QLabel(QString("Replaying: %1 vs %2\nTotal moves: %3")
                                       .arg(player1Name)
                                       .arg(opponent)
                                       .arg(moves.size()), replayDialog);
    infoLabel->setAlignment(Qt::AlignCenter);

    QLabel* moveLabel = new QLabel("Press 'Next' to see each move", replayDialog);
    moveLabel->setAlignment(Qt::AlignCenter);

    QString labelStyle = R"(
        QLabel {
            color: white;
            font-size: 16px;
            font-weight: bold;
            text-shadow: 0 0 15px #FF1493;
            background: rgba(0, 0, 0, 0.7);
            border: 2px solid #FF1493;
            border-radius: 12px;
            padding: 15px;
        }
    )";

    titleLabel->setStyleSheet(labelStyle);
    infoLabel->setStyleSheet(labelStyle);
    moveLabel->setStyleSheet(labelStyle);

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    QPushButton* nextButton = new QPushButton("▶️ Next Move", replayDialog);
    QPushButton* autoButton = new QPushButton("⏩ Auto Play", replayDialog);
    QPushButton* closeButton = new QPushButton("✕ Close", replayDialog);

    QString buttonStyle = R"(
        QPushButton {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                stop:0 rgba(255, 20, 147, 0.9),
                stop:1 rgba(138, 43, 226, 0.9));
            color: white;
            border: 2px solid #FF1493;
            border-radius: 12px;
            font-size: 14px;
            font-weight: bold;
            padding: 10px 15px;
        }
        QPushButton:hover {
            border: 2px solid #00FFFF;
            text-shadow: 0 0 10px #00FFFF;
        }
    )";

    nextButton->setStyleSheet(buttonStyle);
    autoButton->setStyleSheet(buttonStyle);
    closeButton->setStyleSheet(buttonStyle);

    buttonLayout->addWidget(nextButton);
    buttonLayout->addWidget(autoButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(closeButton);

    layout->addWidget(titleLabel);
    layout->addWidget(infoLabel);
    layout->addWidget(moveLabel);
    layout->addLayout(buttonLayout);

    int currentMoveIndex = 0;
    QTimer* autoTimer = new QTimer(replayDialog);
    autoTimer->setSingleShot(false);
    autoTimer->setInterval(1500); // 1.5 seconds between moves

    auto playNextMove = [=]() mutable {
        if (currentMoveIndex < moves.size()) {
            QString move = moves[currentMoveIndex].toString();
            char player = move[0].toLatin1();
            int row = move[1].digitValue();
            int col = move[2].digitValue();

            // Make the move on the board
            board->makeMove(row, col, player);
            gameButtons[row][col]->setText(QString(player));

            // Apply color styling
            if (player == 'X') {
                gameButtons[row][col]->setStyleSheet(gameButtons[row][col]->styleSheet() +
                                                     " color: #FF1493; text-shadow: 0 0 15px #FF1493;");
            } else {
                gameButtons[row][col]->setStyleSheet(gameButtons[row][col]->styleSheet() +
                                                     " color: #00FFFF; text-shadow: 0 0 15px #00FFFF;");
            }

            currentMoveIndex++;
            moveLabel->setText(QString("Move %1/%2: Player %3 at (%4,%5)")
                                   .arg(currentMoveIndex)
                                   .arg(moves.size())
                                   .arg(player)
                                   .arg(row + 1)
                                   .arg(col + 1));

            if (currentMoveIndex >= moves.size()) {
                nextButton->setEnabled(false);
                autoTimer->stop();
                autoButton->setText("✅ Replay Complete");
                autoButton->setEnabled(false);
            }
        }
    };

    connect(nextButton, &QPushButton::clicked, playNextMove);

    connect(autoButton, &QPushButton::clicked, [=]() mutable {
        if (autoTimer->isActive()) {
            autoTimer->stop();
            autoButton->setText("⏩ Auto Play");
        } else {
            autoTimer->start();
            autoButton->setText("⏸️ Pause");
        }
    });

    connect(autoTimer, &QTimer::timeout, playNextMove);
    connect(closeButton, &QPushButton::clicked, replayDialog, &QDialog::accept);

    replayDialog->exec();
    delete replayDialog;
}

void MainWindow::deleteGameFromHistory(int gameIndex)
{
    QFile file("game_history.json");
    QJsonObject history;

    if (file.open(QIODevice::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        history = doc.object();
        file.close();
    }

    if (history.contains(player1Name)) {
        QJsonArray userGames = history[player1Name].toArray();

        if (gameIndex >= 0 && gameIndex < userGames.size()) {
            userGames.removeAt(gameIndex);
            history[player1Name] = userGames;

            if (file.open(QIODevice::WriteOnly)) {
                QJsonDocument doc(history);
                file.write(doc.toJson());
                file.close();
            }
        }
    }
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
