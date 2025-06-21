#include "../../includes/mainwindow.h"
#include <QApplication>
#include <QScreen>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), currentPlayer("X"), gameActive(true), currentStep(0), isFullScreen(true)
{
    // Initialize pointers to nullptr first
    stackedWidget = nullptr;
    loginWidget = nullptr;
    gameWidget = nullptr;
    board = nullptr;
    aiPlayer = nullptr;  // Single AI player
    aiTimer = nullptr;
    toolBar = nullptr;

    setWindowTitle("Tic Tac Toe - Synthwave Edition");

    // Initialize objects safely
    board = new Board();
    aiPlayer = new AIPlayer(AIPlayer::MEDIUM);  // Default to hard difficulty
    aiTimer = new QTimer(this);
    aiTimer->setSingleShot(true);

    // Setup UI BEFORE setting window properties
    setupUI();
    setupToolbar();
    setupStyling();

    // NOW set window properties
    setWindowState(Qt::WindowFullScreen);
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    QScreen *screen = QApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    setGeometry(screenGeometry);

    setBackgroundImage();

    // Start with login view
    if (stackedWidget && loginWidget) {
        stackedWidget->setCurrentWidget(loginWidget);
        resetToModeSelection();
    }

    connect(aiTimer, &QTimer::timeout, this, &MainWindow::makeAIMove);
}


void MainWindow::setupLoginView()
{
    loginMainLayout = new QVBoxLayout(loginWidget);
    loginMainLayout->setSpacing(10);  // Reduced spacing
    loginMainLayout->setContentsMargins(40, 20, 40, 20);  // Reduced margins

    // Title
    loginTitleLabel = new QLabel("🎮 TIC TAC TOE 🎮", loginWidget);
    loginTitleLabel->setAlignment(Qt::AlignCenter);
    loginTitleLabel->setObjectName("titleLabel");

    // Create a scroll area for the main content
    QScrollArea* scrollArea = new QScrollArea(loginWidget);
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setStyleSheet("QScrollArea { background: transparent; border: none; }");

    // Create scrollable content widget
    QWidget* scrollContent = new QWidget();
    scrollContent->setObjectName("scrollContent");

    QVBoxLayout* scrollLayout = new QVBoxLayout(scrollContent);
    scrollLayout->setSpacing(12);  // Reduced spacing
    scrollLayout->setContentsMargins(20, 20, 20, 20);

    // Instruction
    instructionLabel = new QLabel("Welcome! Choose your game mode:", scrollContent);
    instructionLabel->setAlignment(Qt::AlignCenter);
    instructionLabel->setObjectName("instructionLabel");

    // Mode selection
    modeComboBox = new QComboBox(scrollContent);
    modeComboBox->addItem("🎯 Player vs Player");
    modeComboBox->addItem("🤖 Player vs AI");
    modeComboBox->setObjectName("modeComboBox");

    continueButton = new QPushButton("Continue", scrollContent);
    continueButton->setObjectName("primaryButton");

    // Player info
    playerLabel = new QLabel(scrollContent);
    playerLabel->setAlignment(Qt::AlignCenter);
    playerLabel->setObjectName("playerLabel");

    // Authentication fields
    usernameLabel = new QLabel("Username:", scrollContent);
    usernameLabel->setObjectName("fieldLabel");

    usernameLineEdit = new QLineEdit(scrollContent);
    usernameLineEdit->setObjectName("inputField");
    usernameLineEdit->setPlaceholderText("Enter your username");

    passwordLabel = new QLabel("Password:", scrollContent);
    passwordLabel->setObjectName("fieldLabel");

    passwordLineEdit = new QLineEdit(scrollContent);
    passwordLineEdit->setObjectName("inputField");
    passwordLineEdit->setEchoMode(QLineEdit::Password);
    passwordLineEdit->setPlaceholderText("Enter your password");

    confirmPasswordLabel = new QLabel("Confirm Password:", scrollContent);
    confirmPasswordLabel->setObjectName("fieldLabel");

    confirmPasswordLineEdit = new QLineEdit(scrollContent);
    confirmPasswordLineEdit->setObjectName("inputField");
    confirmPasswordLineEdit->setEchoMode(QLineEdit::Password);
    confirmPasswordLineEdit->setPlaceholderText("Confirm your password");

    // Buttons
    signInButton = new QPushButton("Sign In", scrollContent);
    signInButton->setObjectName("primaryButton");

    newPlayerButton = new QPushButton("New Player", scrollContent);
    newPlayerButton->setObjectName("secondaryButton");

    nextPlayerButton = new QPushButton("Next Player", scrollContent);
    nextPlayerButton->setObjectName("primaryButton");

    backButton = new QPushButton("← Back", scrollContent);
    backButton->setObjectName("backButton");

    // Game start
    gameInfoLabel = new QLabel(scrollContent);
    gameInfoLabel->setAlignment(Qt::AlignCenter);
    gameInfoLabel->setObjectName("gameInfoLabel");

    startGameButton = new QPushButton("🚀 START GAME", scrollContent);
    startGameButton->setObjectName("startButton");

    // Add widgets to scroll layout with reduced spacing
    scrollLayout->addWidget(instructionLabel);
    scrollLayout->addSpacing(8);
    scrollLayout->addWidget(modeComboBox);
    scrollLayout->addSpacing(8);
    scrollLayout->addWidget(continueButton);
    scrollLayout->addSpacing(10);
    scrollLayout->addWidget(playerLabel);
    scrollLayout->addSpacing(8);
    scrollLayout->addWidget(usernameLabel);
    scrollLayout->addWidget(usernameLineEdit);
    scrollLayout->addSpacing(5);
    scrollLayout->addWidget(passwordLabel);
    scrollLayout->addWidget(passwordLineEdit);
    scrollLayout->addSpacing(5);
    scrollLayout->addWidget(confirmPasswordLabel);
    scrollLayout->addWidget(confirmPasswordLineEdit);
    scrollLayout->addSpacing(10);

    // Button layout with reduced spacing
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(10);
    buttonLayout->addWidget(backButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(signInButton);
    buttonLayout->addWidget(newPlayerButton);
    buttonLayout->addWidget(nextPlayerButton);
    scrollLayout->addLayout(buttonLayout);

    scrollLayout->addSpacing(15);
    scrollLayout->addWidget(gameInfoLabel);
    scrollLayout->addSpacing(10);
    scrollLayout->addWidget(startGameButton);
    scrollLayout->addSpacing(20);  // Bottom padding

    // Set scroll content
    scrollArea->setWidget(scrollContent);

    // Control buttons (fixed at top)
    exitButton = new QPushButton("✕ Exit", loginWidget);
    exitButton->setObjectName("exitButton");

    fullScreenToggleButton = new QPushButton("⛶ Window", loginWidget);
    fullScreenToggleButton->setObjectName("toggleButton");
    fullScreenToggleButton->setToolTip("Press F11 or click to toggle full screen");

    // Create top control buttons layout
    QHBoxLayout* topControlLayout = new QHBoxLayout();
    topControlLayout->setContentsMargins(0, 10, 10, 0);
    topControlLayout->addStretch();
    topControlLayout->addWidget(fullScreenToggleButton);
    topControlLayout->addWidget(exitButton);

    // Add everything to main layout
    loginMainLayout->addWidget(loginTitleLabel);
    loginMainLayout->addLayout(topControlLayout);
    loginMainLayout->addWidget(scrollArea, 1);  // Give scroll area most space

    // Connect login signals
    connect(continueButton, &QPushButton::clicked, this, &MainWindow::onModeChanged);
    connect(signInButton, &QPushButton::clicked, this, &MainWindow::onSignInClicked);
    connect(newPlayerButton, &QPushButton::clicked, this, &MainWindow::onNewPlayerClicked);
    connect(nextPlayerButton, &QPushButton::clicked, this, &MainWindow::onNextPlayerClicked);
    connect(startGameButton, &QPushButton::clicked, this, &MainWindow::onStartGameClicked);
    connect(backButton, &QPushButton::clicked, this, &MainWindow::onBackClicked);
    connect(exitButton, &QPushButton::clicked, this, &MainWindow::onExitClicked);
    connect(fullScreenToggleButton, &QPushButton::clicked, this, &MainWindow::toggleFullScreen);
}
void MainWindow::setupUI()
{
    // Create stacked widget as central widget
    stackedWidget = new QStackedWidget(this);
    setCentralWidget(stackedWidget);

    // Create login and game widgets
    loginWidget = new QWidget();
    gameWidget = new QWidget();

    stackedWidget->addWidget(loginWidget);
    stackedWidget->addWidget(gameWidget);

    setupLoginView();
    setupGameView();
    updateLayoutForMode();
}


void MainWindow::setupGameView()
{
    gameMainLayout = new QHBoxLayout(gameWidget);
    gameMainLayout->setSpacing(20);
    gameMainLayout->setContentsMargins(20, 20, 20, 20);

    // Create left panel for banners
    leftPanel = new QWidget(gameWidget);
    leftPanel->setObjectName("leftPanel");
    leftPanel->setFixedWidth(350);
    leftPanel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

    QVBoxLayout* leftLayout = new QVBoxLayout(leftPanel);
    leftLayout->setSpacing(10);
    leftLayout->setContentsMargins(10, 20, 10, 10);

    // Title (more square-shaped)
    titleLabel = new QLabel("🎮 TIC TAC TOE 🎮", leftPanel);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setObjectName("gameTitleLabel");
    titleLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    // Players info (more square-shaped)
    playersLabel = new QLabel(leftPanel);
    playersLabel->setAlignment(Qt::AlignCenter);
    playersLabel->setObjectName("playersLabel");
    playersLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    // Status (more square-shaped)
    statusLabel = new QLabel("Player X's Turn", leftPanel);
    statusLabel->setAlignment(Qt::AlignCenter);
    statusLabel->setObjectName("statusLabel");
    statusLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    // Difficulty selection (NEW!)
    difficultyLabel = new QLabel("🎯 AI Difficulty:", leftPanel);
    difficultyLabel->setAlignment(Qt::AlignCenter);
    difficultyLabel->setObjectName("difficultyLabel");
    difficultyLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    difficultyComboBox = new QComboBox(leftPanel);
    difficultyComboBox->addItem("😊 Easy");
    difficultyComboBox->addItem("🤔 Medium");
    difficultyComboBox->addItem("😈 Hard");
    difficultyComboBox->setCurrentIndex(1); // Default to Hard
    difficultyComboBox->setObjectName("difficultyComboBox");

    // Add banners to left panel with reduced spacing
    leftLayout->addSpacing(10);
    leftLayout->addWidget(titleLabel);
    leftLayout->addSpacing(5);
    leftLayout->addWidget(playersLabel);
    leftLayout->addSpacing(5);
    leftLayout->addWidget(statusLabel);
    leftLayout->addSpacing(8);
    leftLayout->addWidget(difficultyLabel);
    leftLayout->addWidget(difficultyComboBox);
    leftLayout->addStretch();

    // Create right panel for game grid
    rightPanel = new QWidget(gameWidget);
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
    gameMainLayout->addWidget(leftPanel);
    gameMainLayout->addWidget(rightPanel);

    // Connect difficulty change signal
    connect(difficultyComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onDifficultyChanged);
}



void MainWindow::setupGameGrid()
{
    gameGridWidget = new QWidget(rightPanel);
    gameGridWidget->setObjectName("gameGridWidget");

    gameGridWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    gameGridWidget->setMinimumSize(500, 500);
    gameGridWidget->setMaximumSize(800, 800);

    gameGridLayout = new QGridLayout(gameGridWidget);
    gameGridLayout->setSpacing(20);
    gameGridLayout->setContentsMargins(30, 30, 30, 30);

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            gameButtons[i][j] = new QPushButton("", gameGridWidget);
            gameButtons[i][j]->setObjectName("gameButton");

            gameButtons[i][j]->setMinimumSize(140, 140);
            gameButtons[i][j]->setMaximumSize(200, 200);

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

void MainWindow::setupToolbar()
{
    toolBar = addToolBar("Game Controls");
    toolBar->setMovable(false);
    toolBar->setFloatable(false);
    toolBar->setVisible(false);

    // Hide toolbar initially (will be shown only in game view)
    toolBar->setVisible(false);

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
void MainWindow::onDifficultyChanged(int index)
{
    // Only show difficulty for AI games
    if (gameMode != "PvAI" || !aiPlayer) {
        return;
    }

    // Switch AI difficulty
    QString message;
    switch (index) {
    case 0: // Easy
        aiPlayer->setDifficulty(AIPlayer::EASY);
        message = "🎮 Switched to Easy Mode!\n\nDon't Defeat Me! Let's be friends 😊. Starting a new game...";
        break;

    case 1: // Medium
        aiPlayer->setDifficulty(AIPlayer::MEDIUM);
        message = "🎮 Switched to Medium Mode!\n\nHeyy Champ! I will give No Mercy 🤔 . A good challenge! Starting a new game...";
        break;

    case 2: // Hard
    default:
        aiPlayer->setDifficulty(AIPlayer::HARD);
        message = "🎮 Switched to Hard Mode!\n\nHeyy Champ! You Don't have a chance 😈 . Good luck! Starting a new game...";
        break;
    }

    QMessageBox::information(this, "Difficulty Changed", message);

    // Start new game with new difficulty
    resetGame();
}





void MainWindow::updateLayoutForMode()
{
    if (isFullScreen) {
        loginMainLayout->setSpacing(20);
        loginMainLayout->setContentsMargins(60, 40, 60, 40);
        if (findChild<QWidget*>("centerWidget")) {
            findChild<QWidget*>("centerWidget")->setMaximumWidth(800);
        }
    } else {
        loginMainLayout->setSpacing(15);
        loginMainLayout->setContentsMargins(30, 20, 30, 20);
        if (findChild<QWidget*>("centerWidget")) {
            findChild<QWidget*>("centerWidget")->setMaximumWidth(600);
        }
    }
}

void MainWindow::toggleFullScreen()
{
    if (isFullScreen) {
        // Switch to windowed mode
        setWindowFlags(Qt::Window);
        setWindowState(Qt::WindowNoState);
        resize(1200, 900);

        // Center the window on screen
        QScreen *screen = QApplication::primaryScreen();
        QRect screenGeometry = screen->geometry();
        int x = (screenGeometry.width() - width()) / 2;
        int y = (screenGeometry.height() - height()) / 2;
        move(x, y);

        fullScreenToggleButton->setText("⛶ Full Screen");
        fullScreenToggleButton->setToolTip("Press F11 or click to toggle full screen");
        isFullScreen = false;
    } else {
        // Switch to full screen mode
        setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
        setWindowState(Qt::WindowFullScreen);

        QScreen *screen = QApplication::primaryScreen();
        QRect screenGeometry = screen->geometry();
        setGeometry(screenGeometry);

        fullScreenToggleButton->setText("⛶ Window");
        fullScreenToggleButton->setToolTip("Press F11 or click to toggle windowed mode");
        isFullScreen = true;
    }

    updateLayoutForMode();
    show();
    setBackgroundImage();

}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_F11) {
        toggleFullScreen();
        event->accept();
    } else {
        QMainWindow::keyPressEvent(event);
    }
}

void MainWindow::switchToGameView()
{
    stackedWidget->setCurrentWidget(gameWidget);
    toolBar->setVisible(true);
    QPixmap background;

    // Safe to check current widget
    if (stackedWidget->currentWidget() == loginWidget) {
        background.load("D:/images/login_bg.jpg");
    } else {
        background.load("D:/images/game_bg.jpg");
    }

    if (!background.isNull()) {
        background = background.scaled(this->size(),
                                       Qt::KeepAspectRatioByExpanding,
                                       Qt::SmoothTransformation);

        QPalette palette;
        palette.setBrush(QPalette::Window, background);
        this->setPalette(palette);
        this->setAutoFillBackground(true);
    }
    // Show/hide difficulty based on game mode
    if (difficultyLabel && difficultyComboBox) {
        bool showDifficulty = (gameMode == "PvAI");
        difficultyLabel->setVisible(showDifficulty);
        difficultyComboBox->setVisible(showDifficulty);
    }
    resetGame();
    updateGameStatus();
}

// Login methods (same as before)
void MainWindow::resetToModeSelection()
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

void MainWindow::onModeChanged()
{
    gameMode = modeComboBox->currentIndex() == 0 ? "PvP" : "PvAI";
    showPlayer1Auth();
}

void MainWindow::showPlayer1Auth()
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

void MainWindow::showPlayer2Auth()
{
    currentStep = 2;

    playerLabel->setText("🎮 Player 2 Authentication");
    usernameLineEdit->clear();
    passwordLineEdit->clear();
    confirmPasswordLineEdit->hide();
    confirmPasswordLabel->hide();
}

void MainWindow::showGameStart()
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

void MainWindow::onSignInClicked()
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

void MainWindow::onNewPlayerClicked()
{
    confirmPasswordLabel->show();
    confirmPasswordLineEdit->show();

    signInButton->setText("Register");
    newPlayerButton->hide();
    nextPlayerButton->show();
    nextPlayerButton->setText("Register");

    disconnect(signInButton, &QPushButton::clicked, this, &MainWindow::onSignInClicked);
    connect(signInButton, &QPushButton::clicked, this, &MainWindow::onNextPlayerClicked);
}

void MainWindow::onNextPlayerClicked()
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

            disconnect(signInButton, &QPushButton::clicked, this, &MainWindow::onNextPlayerClicked);
            connect(signInButton, &QPushButton::clicked, this, &MainWindow::onSignInClicked);

            usernameLineEdit->clear();
            passwordLineEdit->clear();
        } else {
            QMessageBox::warning(this, "Registration Failed", "Username already exists.");
        }
    }
}

void MainWindow::onStartGameClicked()
{

    // Set up players info for game view
    QString playersText = QString("👤 %1 (X) vs %2 (O)")
                              .arg(player1Name)
                              .arg(player2Name);

    if (gameMode == "PvAI") {
        playersText = QString("👤 %1 (X) vs 🤖 AI (O)").arg(player1Name);
    }

    playersLabel->setText(playersText);

    // Switch to game view
    switchToGameView();
}

void MainWindow::onBackClicked()
{
    resetToModeSelection();
}

void MainWindow::onExitClicked()
{
    QApplication::quit();
}

void MainWindow::onLogoutClicked()
{
    int ret = QMessageBox::question(this, "Logout",
                                    "Are you sure you want to logout?",
                                    QMessageBox::Yes | QMessageBox::No);

    if (ret == QMessageBox::Yes) {
        stackedWidget->setCurrentWidget(loginWidget);
        toolBar->setVisible(false);  // Hide toolbar in login view
        resetToModeSelection();
    }
    QPixmap background;

    // Safe to check current widget
    if (stackedWidget->currentWidget() == loginWidget) {
        background.load("D:/images/login_bg.jpg");
    } else {
        background.load("D:/images/login_bg.jpg");
    }

    if (!background.isNull()) {
        background = background.scaled(this->size(),
                                       Qt::KeepAspectRatioByExpanding,
                                       Qt::SmoothTransformation);

        QPalette palette;
        palette.setBrush(QPalette::Window, background);
        this->setPalette(palette);
        this->setAutoFillBackground(true);
    }
}

// Authentication methods (same as before)
bool MainWindow::authenticateUser(const QString& username, const QString& password)
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

bool MainWindow::registerUser(const QString& username, const QString& password)
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

QString MainWindow::hashPassword(const QString& password)
{
    return QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex();
}

// Game methods (keep all existing game methods exactly the same)
void MainWindow::onGameButtonClicked()
{
   if (!gameActive || !board) return;  // Add board null check

    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (!button || !button->text().isEmpty()) return;

    int row = button->property("row").toInt();
    int col = button->property("col").toInt();

    if (board->makeMove(row, col, currentPlayer.at(0).toLatin1())) {
        button->setText(currentPlayer);

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

            if (gameMode == "PvAI" && currentPlayer == "O") {
                statusLabel->setText("🤖 AI is thinking...");
                aiTimer->start(100);
            }
        }
    }
}

void MainWindow::makeAIMove()
{
    if (!gameActive || currentPlayer != "O" || !board || !aiPlayer) return;

    auto availableMoves = board->getAvailableMoves();
    if (!availableMoves.empty()) {
        auto move = aiPlayer->getMove(board);  // Uses current difficulty setting
        int row = move.first;
        int col = move.second;

        // Validate move coordinates
        if (row >= 0 && row < 3 && col >= 0 && col < 3) {
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
}


void MainWindow::animateButton(QPushButton* button)
{
    if (!button) return;

    QPropertyAnimation* animation = new QPropertyAnimation(button, "geometry");
    animation->setDuration(150);

    QRect originalGeometry = button->geometry();
    QRect scaledGeometry = originalGeometry.adjusted(-3, -3, 3, 3);

    animation->setStartValue(originalGeometry);
    animation->setEndValue(scaledGeometry);
    animation->setEasingCurve(QEasingCurve::OutBounce);

    QPropertyAnimation* returnAnimation = new QPropertyAnimation(button, "geometry");
    returnAnimation->setDuration(150);
    returnAnimation->setStartValue(scaledGeometry);
    returnAnimation->setEndValue(originalGeometry);
    returnAnimation->setEasingCurve(QEasingCurve::InBounce);

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
        winner = 'T';
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

        QTimer::singleShot(100, [this, result]() {
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

    QPixmap xoxoBackground("D:/images/gameover_bg.jpg");
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

// Keep all existing history methods exactly the same
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
    QFile file("game_history.json");
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

    // Create larger replay dialog
    QDialog* replayDialog = new QDialog(this);
    replayDialog->setWindowTitle("Game Replay");
    replayDialog->setFixedSize(600, 450);  // Increased from 400x300 to 600x450
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
    layout->setSpacing(25);  // Increased spacing
    layout->setContentsMargins(40, 40, 40, 40);  // Larger margins

    QLabel* titleLabel = new QLabel("🎬 Game Replay", replayDialog);
    titleLabel->setAlignment(Qt::AlignCenter);

    QLabel* infoLabel = new QLabel(QString("Replaying: %1 vs %2\nTotal moves: %3")
                                       .arg(player1Name)
                                       .arg(opponent)
                                       .arg(moves.size()), replayDialog);
    infoLabel->setAlignment(Qt::AlignCenter);

    QLabel* moveLabel = new QLabel("Press 'Next Move' to see each move", replayDialog);
    moveLabel->setAlignment(Qt::AlignCenter);

    QString labelStyle = R"(
        QLabel {

            color: white;
            font-size: 18px;  /* Increased font size */
            font-weight: bold;
            text-shadow: 0 0 15px #FF1493;
            background: rgba(0, 0, 0, 0.7);
            border: 2px solid #FF1493;
            border-radius: 12px;
            padding: 10px;  /* Increased padding */
            margin: 5px;
        }
    )";

    titleLabel->setStyleSheet(labelStyle);
    infoLabel->setStyleSheet(labelStyle);
    moveLabel->setStyleSheet(labelStyle);

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(20);  // Increased spacing between buttons

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
            border-radius: 15px;  /* Increased border radius */
            font-size: 16px;      /* Increased font size */
            font-weight: bold;
            padding: 10px 20px;   /* Increased padding for larger buttons */
            min-width: 120px;     /* Minimum width to ensure text fits */
            min-height: 45px;     /* Minimum height for better visibility */
        }
        QPushButton:hover {
            border: 2px solid #00FFFF;
            text-shadow: 0 0 10px #00FFFF;
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                stop:0 rgba(138, 43, 226, 1.0),
                stop:1 rgba(255, 20, 147, 1.0));
        }
        QPushButton:pressed {
            background: rgba(75, 0, 130, 0.9);
            border: 2px solid #8A2BE2;
        }
    )";

    nextButton->setStyleSheet(buttonStyle);
    autoButton->setStyleSheet(buttonStyle);
    closeButton->setStyleSheet(buttonStyle);

    buttonLayout->addWidget(nextButton);
    buttonLayout->addWidget(autoButton);
    buttonLayout->addStretch();  // Add stretch to separate close button
    buttonLayout->addWidget(closeButton);

    layout->addWidget(titleLabel);
    layout->addSpacing(10);  // Extra spacing
    layout->addWidget(infoLabel);
    layout->addSpacing(10);  // Extra spacing
    layout->addWidget(moveLabel);
    layout->addSpacing(10);  // Extra spacing before buttons
    layout->addLayout(buttonLayout);
    layout->addStretch();    // Push everything up, leaving space at bottom

    int currentMoveIndex = 0;
    QTimer* autoTimer = new QTimer(replayDialog);
    autoTimer->setSingleShot(false);
    autoTimer->setInterval(1000); // 1 seconds between moves

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
            moveLabel->setText(QString("Move %1/%2: Player %3 at position (%4,%5)")
                                   .arg(currentMoveIndex)
                                   .arg(moves.size())
                                   .arg(player)
                                   .arg(row + 1)
                                   .arg(col + 1));

            if (currentMoveIndex >= moves.size()) {
                nextButton->setEnabled(false);
                nextButton->setText("✅ Complete");
                autoTimer->stop();
                autoButton->setText("✅ Replay Complete");
                autoButton->setEnabled(false);
                moveLabel->setText("🎉 Replay finished! All moves have been shown.");
            }
        }
    };

    connect(nextButton, &QPushButton::clicked, playNextMove);

    connect(autoButton, &QPushButton::clicked, [=]() mutable {
        if (autoTimer->isActive()) {
            autoTimer->stop();
            autoButton->setText("⏩ Auto Play");
        } else {
            if (currentMoveIndex < moves.size()) {
                autoTimer->start();
                autoButton->setText("⏸️ Pause");
            }
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

void MainWindow::resetGame()
{
    board->reset();
    gameActive = true;
    currentPlayer = "X";
    moveHistory.clear();

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            gameButtons[i][j]->setText("");
            gameButtons[i][j]->setStyleSheet("");
        }
    }

    updateGameStatus();
}

void MainWindow::setupStyling()
{
    QString style = R"(
        QMainWindow {
            background-color: rgba(0, 0, 0, 0.1);
        }

        #scrollContent {
            background: transparent;
        }

        QScrollArea {
            background: transparent;
            border: none;
        }

        QScrollBar:vertical {
            background: rgba(0, 0, 0, 0.3);
            width: 12px;
            border-radius: 6px;
        }

        QScrollBar::handle:vertical {
            background: rgba(138, 43, 226, 0.8);
            border-radius: 6px;
            min-height: 20px;
        }

        QScrollBar::handle:vertical:hover {
            background: rgba(255, 20, 147, 0.8);
        }

        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
            height: 0px;
        }
        #titleLabel {
            color: #FFD700;
            font-size: 42px;
            font-weight: bold;
            text-shadow: 0 0 30px #FFD700, 0 0 40px #FFD700;
            background: rgba(25, 25, 112, 0.8);
            border: 4px solid #FFD700;
            border-radius: 25px;
            padding: 25px;
            margin: 15px;
        }

        #gameTitleLabel {
            color: #00FFFF;
            font-size: 20px;
            font-weight: bold;
            text-shadow: 0 0 15px #00FFFF, 0 0 20px #00FFFF;
            background: rgba(0, 0, 0, 0.8);
            border: 2px solid #00FFFF;
            border-radius: 15px;
            padding: 20px;
            margin: 5px;
            min-height: 60px;
        }

        #instructionLabel, #playerLabel, #gameInfoLabel {
            color: white;
            font-size: 20px;
            font-weight: bold;
            text-shadow: 2px 2px 4px rgba(0,0,0,0.9);
            background: rgba(75, 0, 130, 0.7);
            border-radius: 15px;
            padding: 15px;
            border: 3px solid #9370DB;
            margin: 5px 0;
        }

        #playersLabel {
            color: #FF1493;
            font-size: 16px;
            font-weight: bold;
            text-shadow: 0 0 8px #FF1493;
            background: rgba(0, 0, 0, 0.7);
            border: 2px solid #FF1493;
            border-radius: 15px;
            padding: 20px;
            margin: 5px;
            min-height: 60px;
        }

        #statusLabel {
            color: #00FFFF;
            font-size: 18px;
            font-weight: bold;
            text-shadow: 0 0 10px #00FFFF;
            background: rgba(0, 0, 0, 0.8);
            border: 2px solid #00FFFF;
            border-radius: 15px;
            padding: 20px;
            margin: 5px;
            min-height: 60px;
        }

        #fieldLabel {
            color: #E6E6FA;
            font-size: 16px;
            font-weight: bold;
            text-shadow: 1px 1px 2px rgba(0,0,0,0.8);
            margin: 5px 0;
        }

        #inputField {
            background: rgba(25, 25, 112, 0.9);
            color: white;
            border: 3px solid #4169E1;
            border-radius: 12px;
            padding: 15px 18px;
            font-size: 15px;
            selection-background-color: rgba(138, 43, 226, 0.7);
            min-height: 18px;
            margin: 3px 0;
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
            padding: 15px 18px;
            font-size: 16px;
            font-weight: bold;
            min-height: 22px;
            margin: 5px 0;
        }

        #primaryButton {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 rgba(138, 43, 226, 0.9),
                stop:1 rgba(75, 0, 130, 0.9));
            color: white;
            border: 3px solid #FFD700;
            border-radius: 18px;
            font-size: 16px;
            font-weight: bold;
            padding: 15px 30px;
            text-shadow: 1px 1px 2px rgba(0,0,0,0.8);
            min-height: 20px;
            min-width: 120px;
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
            font-size: 16px;
            font-weight: bold;
            padding: 15px 30px;
            text-shadow: 1px 1px 2px rgba(0,0,0,0.8);
            min-height: 20px;
            min-width: 120px;
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
            font-size: 14px;
            font-weight: bold;
            padding: 12px 20px;
            min-width: 80px;
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
            padding: 12px 20px;
            margin: 10px;
        }

        #exitButton:hover {
            background: rgba(139, 0, 0, 1.0);
            border: 3px solid #FF0000;
        }

        #toggleButton {
            background: rgba(0, 100, 0, 0.8);
            color: white;
            border: 3px solid #228B22;
            border-radius: 15px;
            font-size: 14px;
            font-weight: bold;
            padding: 12px 20px;
            margin: 10px;
        }

        #toggleButton:hover {
            background: rgba(0, 128, 0, 1.0);
            border: 3px solid #32CD32;
        }

        #startButton {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                stop:0 rgba(255, 215, 0, 0.9),
                stop:0.5 rgba(255, 140, 0, 0.9),
                stop:1 rgba(255, 69, 0, 0.9));
            color: #8B0000;
            border: 4px solid #FFD700;
            border-radius: 25px;
            font-size: 22px;
            font-weight: bold;
            padding: 20px 40px;
            text-shadow: 1px 1px 2px rgba(255,255,255,0.8);
            margin: 10px 0;
        }

        #startButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                stop:0 rgba(255, 215, 0, 1.0),
                stop:0.5 rgba(255, 140, 0, 1.0),
                stop:1 rgba(255, 69, 0, 1.0));
            border: 4px solid #FFA500;
        }

        #leftPanel {
            background: rgba(0, 0, 0, 0.3);
            border: 2px solid #8A2BE2;
            border-radius: 15px;
        }

        #rightPanel {
            background: rgba(0, 0, 0, 0.1);
        }

        #gameGridWidget {
            background: rgba(0, 0, 0, 0.6);
            border: 4px solid #8A2BE2;
            border-radius: 25px;
        }

        #gameButton {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                stop:0 rgba(0, 0, 0, 0.8),
                stop:1 rgba(75, 0, 130, 0.6));
            border: 3px solid #00FFFF;
            border-radius: 15px;
            color: white;
            font-size: 56px;
            font-weight: bold;
            text-shadow: 0 0 15px currentColor;
        }

        #gameButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                stop:0 rgba(255, 20, 147, 0.4),
                stop:1 rgba(138, 43, 226, 0.4));
            border: 4px solid #FF1493;
            text-shadow: 0 0 25px currentColor;
        }

        #gameButton:pressed {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                stop:0 rgba(138, 43, 226, 0.8),
                stop:1 rgba(75, 0, 130, 0.8));
            border: 3px solid #8A2BE2;
        }

QToolBar {
            background: transparent;  /* Transparent background */
            border: none;             /* No border */
            spacing: 15px;
            padding: 5px;             /* Reduced padding */
        }

        QToolButton {
            background: rgba(0, 0, 0, 0.8);  /* Semi-transparent background */
            color: white;
            border: 2px solid #00FFFF;
            border-radius: 8px;
            padding: 8px 15px;
            font-weight: bold;
            font-size: 12px;
            text-shadow: 0 0 5px rgba(255, 255, 255, 0.8);
            margin: 2px;  /* Small margin between buttons */
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
#difficultyLabel {
    color: #FFD700;
    font-size: 14px;
    font-weight: bold;
    text-shadow: 0 0 8px #FFD700;
    background: rgba(0, 0, 0, 0.7);
    border: 2px solid #FFD700;
    border-radius: 15px;
    padding: 12px;
    margin: 3px;
    min-height: 40px;
}

#difficultyComboBox {
    background: rgba(75, 0, 130, 0.9);
    color: white;
    border: 2px solid #9370DB;
    border-radius: 12px;
    padding: 10px 15px;
    font-size: 14px;
    font-weight: bold;
    min-height: 20px;
    margin: 3px;
}

#difficultyComboBox::drop-down {
    border: none;
    background: rgba(138, 43, 226, 0.8);
    border-radius: 5px;
}

#difficultyComboBox::down-arrow {
    image: none;
    border: 4px solid transparent;
    border-top: 6px solid white;
    margin-right: 8px;
}

#difficultyComboBox QAbstractItemView {
    background: rgba(75, 0, 130, 0.95);
    color: white;
    border: 2px solid #9370DB;
    border-radius: 8px;
    selection-background-color: rgba(255, 20, 147, 0.8);
}


    )";

    this->setStyleSheet(style);
}

void MainWindow::setBackgroundImage()
{
    QPixmap background;

        // Safe to check current widget
        if (stackedWidget->currentWidget() == loginWidget) {
            background.load("D:/images/login_bg.jpg");
        } else {
            background.load("D:/images/game_bg.jpg");
        }

    if (!background.isNull()) {
        background = background.scaled(this->size(),
                                       Qt::KeepAspectRatioByExpanding,
                                       Qt::SmoothTransformation);

        QPalette palette;
        palette.setBrush(QPalette::Window, background);
        this->setPalette(palette);
        this->setAutoFillBackground(true);
    }
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event);

    // Only call setBackgroundImage if widgets are initialized
    if (stackedWidget && loginWidget && gameWidget) {
        setBackgroundImage();
    }
}

