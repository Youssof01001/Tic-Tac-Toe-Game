#include "LoginDialog.h"
#include <QMessageBox>
#include <QCryptographicHash>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QApplication>

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent), selectedMode(0), registering(false) {
    setWindowTitle("Tic Tac Toe - Game Setup");
    setFixedSize(500, 400);

    stackedWidget = new QStackedWidget(this);

    setupModeSelectionPage();
    setupAuthTypeSelectionPage();
    setupLoginPage();
    setupRegisterPage();
    setupPlayer2AuthTypePage();
    setupPlayer2LoginPage();
    setupPlayer2RegisterPage();

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(stackedWidget);

    statusLabel = new QLabel(this);
    statusLabel->setAlignment(Qt::AlignCenter);
    statusLabel->setStyleSheet("color: red; font-weight: bold;");
    mainLayout->addWidget(statusLabel);

    applyStyles();
    stackedWidget->setCurrentWidget(modeSelectionPage);
}

void LoginDialog::setupModeSelectionPage() {
    modeSelectionPage = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(modeSelectionPage);

    QLabel *titleLabel = new QLabel("Choose Game Mode");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #2c3e50; margin: 20px;");
    layout->addWidget(titleLabel);

    layout->addStretch();

    twoPlayersButton = new QPushButton("Two Players");
    twoPlayersButton->setFixedSize(200, 60);
    playerVsAIButton = new QPushButton("Player vs AI");
    playerVsAIButton->setFixedSize(200, 60);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(twoPlayersButton);
    buttonLayout->addSpacing(20);
    buttonLayout->addWidget(playerVsAIButton);
    buttonLayout->addStretch();

    layout->addLayout(buttonLayout);
    layout->addStretch();

    // Add quit button
    QPushButton *quitButton = new QPushButton("❌ Quit");
    quitButton->setFixedSize(100, 50);
    quitButton->setStyleSheet(
        "QPushButton {"
        "    background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, "
        "    stop:0 #e74c3c, stop:1 #c0392b);"
        "    border: none;"
        "    border-radius: 8px;"
        "    color: white;"
        "    font-weight: bold;"
        "    font-size: 12px;"
        "}"
        "QPushButton:hover {"
        "    background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, "
        "    stop:0 #ec7063, stop:1 #e74c3c);"
        "}"
        );

    QHBoxLayout *quitLayout = new QHBoxLayout();
    quitLayout->addStretch();
    quitLayout->addWidget(quitButton);
    layout->addLayout(quitLayout);

    connect(twoPlayersButton, &QPushButton::clicked, this, &LoginDialog::handleTwoPlayersClicked);
    connect(playerVsAIButton, &QPushButton::clicked, this, &LoginDialog::handlePlayerVsAIClicked);
    connect(quitButton, &QPushButton::clicked, [this]() {
        QApplication::quit();
    });

    stackedWidget->addWidget(modeSelectionPage);
}

void LoginDialog::setupAuthTypeSelectionPage() {
    authTypeSelectionPage = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(authTypeSelectionPage);

    QLabel *titleLabel = new QLabel("Player 1 ");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 20px; font-weight: bold; color: #2c3e50; margin: 20px;");
    layout->addWidget(titleLabel);

    layout->addStretch();

    signInButton = new QPushButton("Sign In");
    signInButton->setFixedSize(150, 50);
    newPlayerButton = new QPushButton("New Player");
    newPlayerButton->setFixedSize(150, 50);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(signInButton);
    buttonLayout->addSpacing(20);
    buttonLayout->addWidget(newPlayerButton);
    buttonLayout->addStretch();

    layout->addLayout(buttonLayout);
    layout->addStretch();

    backToModeButton = new QPushButton("← Back");
    backToModeButton->setFixedSize(80, 50);
    QHBoxLayout *backLayout = new QHBoxLayout();
    backLayout->addWidget(backToModeButton);
    backLayout->addStretch();
    layout->addLayout(backLayout);

    connect(signInButton, &QPushButton::clicked, this, &LoginDialog::handleSignInClicked);
    connect(newPlayerButton, &QPushButton::clicked, this, &LoginDialog::handleNewPlayerClicked);
    connect(backToModeButton, &QPushButton::clicked, [this]() {
        stackedWidget->setCurrentWidget(modeSelectionPage);
    });

    stackedWidget->addWidget(authTypeSelectionPage);
}

void LoginDialog::setupLoginPage() {
    loginPage = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(loginPage);

    QLabel *titleLabel = new QLabel("Sign In");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 20px; font-weight: bold; color: #2c3e50; margin: 20px;");
    layout->addWidget(titleLabel);

    layout->addStretch();

    usernameEdit = new QLineEdit();
    usernameEdit->setPlaceholderText("Username");
    usernameEdit->setFixedHeight(50);

    passwordEdit = new QLineEdit();
    passwordEdit->setPlaceholderText("Password");
    passwordEdit->setEchoMode(QLineEdit::Password);
    passwordEdit->setFixedHeight(50);

    loginButton = new QPushButton("Login");
    loginButton->setFixedHeight(45);

    layout->addWidget(usernameEdit);
    layout->addWidget(passwordEdit);
    layout->addSpacing(10);
    layout->addWidget(loginButton);
    layout->addStretch();

    backToAuthButton = new QPushButton("← Back");
    backToAuthButton->setFixedSize(80, 50);
    QHBoxLayout *backLayout = new QHBoxLayout();
    backLayout->addWidget(backToAuthButton);
    backLayout->addStretch();
    layout->addLayout(backLayout);

    connect(loginButton, &QPushButton::clicked, this, &LoginDialog::handleLoginClicked);
    connect(backToAuthButton, &QPushButton::clicked, [this]() {
        stackedWidget->setCurrentWidget(authTypeSelectionPage);
    });

    stackedWidget->addWidget(loginPage);
}

void LoginDialog::setupRegisterPage() {
    registerPage = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(registerPage);

    QLabel *titleLabel = new QLabel("Create New Account");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 20px; font-weight: bold; color: #2c3e50; margin: 10px;");
    layout->addWidget(titleLabel);

    layout->addStretch();

    regUsernameEdit = new QLineEdit();
    regUsernameEdit->setPlaceholderText("Username");
    regUsernameEdit->setFixedHeight(50);

    regPasswordEdit = new QLineEdit();
    regPasswordEdit->setPlaceholderText("Password");
    regPasswordEdit->setEchoMode(QLineEdit::Password);
    regPasswordEdit->setFixedHeight(50);

    regConfirmPasswordEdit = new QLineEdit();
    regConfirmPasswordEdit->setPlaceholderText("Confirm Password");
    regConfirmPasswordEdit->setEchoMode(QLineEdit::Password);
    regConfirmPasswordEdit->setFixedHeight(50);

    registerButton = new QPushButton("Sign Up");
    registerButton->setFixedHeight(50);

    layout->addWidget(regUsernameEdit);
    layout->addWidget(regPasswordEdit);
    layout->addWidget(regConfirmPasswordEdit);
    layout->addSpacing(10);
    layout->addWidget(registerButton);
    layout->addStretch();

    backToAuthButton2 = new QPushButton("← Back");
    backToAuthButton2->setFixedSize(80, 50);
    QHBoxLayout *backLayout = new QHBoxLayout();
    backLayout->addWidget(backToAuthButton2);
    backLayout->addStretch();
    layout->addLayout(backLayout);

    connect(registerButton, &QPushButton::clicked, this, &LoginDialog::handleRegisterClicked);
    connect(backToAuthButton2, &QPushButton::clicked, [this]() {
        stackedWidget->setCurrentWidget(authTypeSelectionPage);
    });

    stackedWidget->addWidget(registerPage);
}

void LoginDialog::setupPlayer2AuthTypePage() {
    player2AuthTypePage = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(player2AuthTypePage);

    QLabel *titleLabel = new QLabel("Player 2 ");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 20px; font-weight: bold; color: #2c3e50; margin: 20px;");
    layout->addWidget(titleLabel);

    player2InfoLabel = new QLabel();
    player2InfoLabel->setAlignment(Qt::AlignCenter);
    player2InfoLabel->setStyleSheet("font-size: 14px; color: #7f8c8d; margin: 10px;");
    layout->addWidget(player2InfoLabel);

    layout->addStretch();

    player2SignInButton = new QPushButton("Sign In");
    player2SignInButton->setFixedSize(150, 50);
    player2NewPlayerButton = new QPushButton("New Player");
    player2NewPlayerButton->setFixedSize(150, 50);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(player2SignInButton);
    buttonLayout->addSpacing(20);
    buttonLayout->addWidget(player2NewPlayerButton);
    buttonLayout->addStretch();

    layout->addLayout(buttonLayout);
    layout->addStretch();

    connect(player2SignInButton, &QPushButton::clicked, this, &LoginDialog::handlePlayer2SignInClicked);
    connect(player2NewPlayerButton, &QPushButton::clicked, this, &LoginDialog::handlePlayer2NewPlayerClicked);

    stackedWidget->addWidget(player2AuthTypePage);
}

void LoginDialog::setupPlayer2LoginPage() {
    player2LoginPage = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(player2LoginPage);

    QLabel *titleLabel = new QLabel("Player 2 Sign In");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 20px; font-weight: bold; color: #2c3e50; margin: 20px;");
    layout->addWidget(titleLabel);

    layout->addStretch();

    player2UsernameEdit = new QLineEdit();
    player2UsernameEdit->setPlaceholderText("Username");
    player2UsernameEdit->setFixedHeight(50);

    player2PasswordEdit = new QLineEdit();
    player2PasswordEdit->setPlaceholderText("Password");
    player2PasswordEdit->setEchoMode(QLineEdit::Password);
    player2PasswordEdit->setFixedHeight(50);

    player2LoginButton = new QPushButton("Login");
    player2LoginButton->setFixedHeight(50);

    layout->addWidget(player2UsernameEdit);
    layout->addWidget(player2PasswordEdit);
    layout->addSpacing(10);
    layout->addWidget(player2LoginButton);
    layout->addStretch();

    backToPlayer2AuthButton = new QPushButton("← Back");
    backToPlayer2AuthButton->setFixedSize(80, 50);
    QHBoxLayout *backLayout = new QHBoxLayout();
    backLayout->addWidget(backToPlayer2AuthButton);
    backLayout->addStretch();
    layout->addLayout(backLayout);

    connect(player2LoginButton, &QPushButton::clicked, this, &LoginDialog::handlePlayer2LoginClicked);
    connect(backToPlayer2AuthButton, &QPushButton::clicked, [this]() {
        stackedWidget->setCurrentWidget(player2AuthTypePage);
    });

    stackedWidget->addWidget(player2LoginPage);
}

void LoginDialog::setupPlayer2RegisterPage() {
    player2RegisterPage = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(player2RegisterPage);

    QLabel *titleLabel = new QLabel("Player 2 - Create New Account");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 20px; font-weight: bold; color: #2c3e50; margin: 10px;");
    layout->addWidget(titleLabel);

    layout->addStretch();

    player2RegUsernameEdit = new QLineEdit();
    player2RegUsernameEdit->setPlaceholderText("Username");
    player2RegUsernameEdit->setFixedHeight(50);

    player2RegPasswordEdit = new QLineEdit();
    player2RegPasswordEdit->setPlaceholderText("Password");
    player2RegPasswordEdit->setEchoMode(QLineEdit::Password);
    player2RegPasswordEdit->setFixedHeight(50);

    player2RegConfirmPasswordEdit = new QLineEdit();
    player2RegConfirmPasswordEdit->setPlaceholderText("Confirm Password");
    player2RegConfirmPasswordEdit->setEchoMode(QLineEdit::Password);
    player2RegConfirmPasswordEdit->setFixedHeight(50);

    player2RegisterButton = new QPushButton("Sign Up");
    player2RegisterButton->setFixedHeight(50);

    layout->addWidget(player2RegUsernameEdit);
    layout->addWidget(player2RegPasswordEdit);
    layout->addWidget(player2RegConfirmPasswordEdit);
    layout->addSpacing(10);
    layout->addWidget(player2RegisterButton);
    layout->addStretch();

    backToPlayer2AuthButton2 = new QPushButton("← Back");
    backToPlayer2AuthButton2->setFixedSize(80, 50);
    QHBoxLayout *backLayout = new QHBoxLayout();
    backLayout->addWidget(backToPlayer2AuthButton2);
    backLayout->addStretch();
    layout->addLayout(backLayout);

    connect(player2RegisterButton, &QPushButton::clicked, this, &LoginDialog::handlePlayer2RegisterClicked);
    connect(backToPlayer2AuthButton2, &QPushButton::clicked, [this]() {
        stackedWidget->setCurrentWidget(player2AuthTypePage);
    });

    stackedWidget->addWidget(player2RegisterPage);
}

void LoginDialog::applyStyles() {
    // Main dialog background
    setStyleSheet(
        "QDialog {"
        "    background: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, "
        "    stop:0 #667eea, stop:1 #764ba2);"
        "}"
        "QPushButton {"
        "    background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, "
        "    stop:0 #4facfe, stop:1 #00f2fe);"
        "    border: none;"
        "    border-radius: 8px;"
        "    color: white;"
        "    font-weight: bold;"
        "    font-size: 14px;"
        "    padding: 8px;"
        "}"
        "QPushButton:hover {"
        "    background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, "
        "    stop:0 #43e97b, stop:1 #38f9d7);"
        "}"
        "QPushButton:pressed {"
        "    background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, "
        "    stop:0 #fa709a, stop:1 #fee140);"
        "}"
        "QLineEdit {"
        "    background-color: rgba(255, 255, 255, 0.95);"
        "    border: 2px solid #bdc3c7;"
        "    border-radius: 8px;"
        "    padding: 8px;"
        "    font-size: 14px;"
        "    font-weight: bold;"
        "    color: #2c3e50;"
        "    margin: 5px;"
        "}"
        "QLineEdit:focus {"
        "    border: 2px solid #3498db;"
        "    background-color: white;"
        "    color: #2c3e50;"
        "}"
        "QWidget {"
        "    background: transparent;"
        "}"
        );

    // Add quit button to mode selection page
    QPushButton *quitButton = new QPushButton("❌ Quit", modeSelectionPage);
    quitButton->setFixedSize(80, 50);
    quitButton->setStyleSheet(
        "QPushButton {"
        "    background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, "
        "    stop:0 #e74c3c, stop:1 #c0392b);"
        "    border: none;"
        "    border-radius: 8px;"
        "    color: white;"
        "    font-weight: bold;"
        "    font-size: 12px;"
        "}"
        "QPushButton:hover {"
        "    background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, "
        "    stop:0 #ec7063, stop:1 #e74c3c);"
        "}"
        );

    // Position quit button in top-right corner
    quitButton->move(modeSelectionPage->width() - 90, 10);

    connect(quitButton, &QPushButton::clicked, [this]() {
        QApplication::quit();
    });
}



// Rest of the implementation methods remain the same...
void LoginDialog::handleTwoPlayersClicked() {
    selectedMode = 1;
    stackedWidget->setCurrentWidget(authTypeSelectionPage);
}

void LoginDialog::handlePlayerVsAIClicked() {
    selectedMode = 2;
    stackedWidget->setCurrentWidget(authTypeSelectionPage);
}

void LoginDialog::handleSignInClicked() {
    stackedWidget->setCurrentWidget(loginPage);
}

void LoginDialog::handleNewPlayerClicked() {
    stackedWidget->setCurrentWidget(registerPage);
}

void LoginDialog::handleLoginClicked() {
    QString username = usernameEdit->text().trimmed();
    QString password = passwordEdit->text();

    if (username.isEmpty() || password.isEmpty()) {
        statusLabel->setText("Please fill in all fields");
        return;
    }

    QString hashed = QString(QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex());

    QFile file("users.json");
    QJsonObject userData;
    if (file.open(QIODevice::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        userData = doc.object();
        file.close();
    }

    if (!userData.contains(username) || userData[username].toString() != hashed) {
        statusLabel->setText("Invalid username or password");
        return;
    }

    currentUsername = username;
    currentPassword = password;
    registering = false;

    if (selectedMode == 1) {
        // Two players mode - need player 2 authentication
        player2InfoLabel->setText(QString("Player 1: %1\nNow authenticate Player 2").arg(username));
        stackedWidget->setCurrentWidget(player2AuthTypePage);
    } else {
        // Player vs AI mode - we're done
        accept();
    }
}

void LoginDialog::handleRegisterClicked() {
    QString username = regUsernameEdit->text().trimmed();
    QString password = regPasswordEdit->text();
    QString confirmPassword = regConfirmPasswordEdit->text();

    if (username.isEmpty() || password.isEmpty() || confirmPassword.isEmpty()) {
        statusLabel->setText("Please fill in all fields");
        return;
    }

    if (password != confirmPassword) {
        statusLabel->setText("Passwords do not match");
        return;
    }

    if (password.length() < 4) {
        statusLabel->setText("Password must be at least 4 characters");
        return;
    }

    QString hashed = QString(QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex());

    QFile file("users.json");
    QJsonObject userData;
    if (file.open(QIODevice::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        userData = doc.object();
        file.close();
    }

    if (userData.contains(username)) {
        statusLabel->setText("Username already exists");
        return;
    }

    userData[username] = hashed;
    if (file.open(QIODevice::WriteOnly)) {
        QJsonDocument outDoc(userData);
        file.write(outDoc.toJson());
        file.close();
    }

    currentUsername = username;
    currentPassword = password;
    registering = true;

    if (selectedMode == 1) {
        // Two players mode - need player 2 authentication
        player2InfoLabel->setText(QString("Player 1: %1\nNow authenticate Player 2").arg(username));
        stackedWidget->setCurrentWidget(player2AuthTypePage);
    } else {
        // Player vs AI mode - we're done
        accept();
    }
}

void LoginDialog::handlePlayer2SignInClicked() {
    stackedWidget->setCurrentWidget(player2LoginPage);
}

void LoginDialog::handlePlayer2NewPlayerClicked() {
    stackedWidget->setCurrentWidget(player2RegisterPage);
}

void LoginDialog::handlePlayer2LoginClicked() {
    QString username = player2UsernameEdit->text().trimmed();
    QString password = player2PasswordEdit->text();

    if (username.isEmpty() || password.isEmpty()) {
        statusLabel->setText("Please fill in all fields");
        return;
    }

    if (username == currentUsername) {
        statusLabel->setText("Player 2 must use a different username");
        return;
    }

    QString hashed = QString(QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex());

    QFile file("users.json");
    QJsonObject userData;
    if (file.open(QIODevice::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        userData = doc.object();
        file.close();
    }

    if (!userData.contains(username) || userData[username].toString() != hashed) {
        statusLabel->setText("Invalid username or password for Player 2");
        return;
    }

    player2Username = username;
    accept();
}

void LoginDialog::handlePlayer2RegisterClicked() {
    QString username = player2RegUsernameEdit->text().trimmed();
    QString password = player2RegPasswordEdit->text();
    QString confirmPassword = player2RegConfirmPasswordEdit->text();

    if (username.isEmpty() || password.isEmpty() || confirmPassword.isEmpty()) {
        statusLabel->setText("Please fill in all fields");
        return;
    }

    if (username == currentUsername) {
        statusLabel->setText("Player 2 must use a different username");
        return;
    }

    if (password != confirmPassword) {
        statusLabel->setText("Passwords do not match");
        return;
    }

    if (password.length() < 4) {
        statusLabel->setText("Password must be at least 4 characters");
        return;
    }

    QString hashed = QString(QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex());

    QFile file("users.json");
    QJsonObject userData;
    if (file.open(QIODevice::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        userData = doc.object();
        file.close();
    }

    if (userData.contains(username)) {
        statusLabel->setText("Username already exists");
        return;
    }

    userData[username] = hashed;
    if (file.open(QIODevice::WriteOnly)) {
        QJsonDocument outDoc(userData);
        file.write(outDoc.toJson());
        file.close();
    }

    player2Username = username;
    accept();
}

QString LoginDialog::getUsername() const {
    return currentUsername;
}

QString LoginDialog::getPassword() const {
    return currentPassword;
}

QString LoginDialog::getPlayer2Username() const {
    return player2Username;
}

int LoginDialog::getSelectedMode() const {
    return selectedMode;
}

bool LoginDialog::isRegistering() const {
    return registering;
}
