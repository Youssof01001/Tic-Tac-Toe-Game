 #include "mainwindow.h"
#include "LoginDialog.h"
#include <QVBoxLayout>
#include <QGridLayout>
#include <QMessageBox>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QCryptographicHash>
#include <QToolBar>
#include <QAction>
#include <QFont>
#include <QDialog>
#include <QListWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), currentPlayer('X'), mode(1), gameOver(false) {
    authenticateUser();
    setupUI();
    resetGame();
}

void MainWindow::authenticateUser() {
    bool authenticated = false;
    while (!authenticated) {
        LoginDialog login(this);
        if (login.exec() == QDialog::Accepted) {
            QString username = login.getUsername();
            QString password = login.getPassword();
            QString hashed = QString(QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex());

            QFile file("users.json");
            QJsonObject userData;
            if (file.open(QIODevice::ReadOnly)) {
                QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
                userData = doc.object();
                file.close();
            }

            if (login.isRegistering()) {
                if (userData.contains(username)) {
                    QMessageBox::warning(this, "Register", "Username already exists!");
                    continue;
                }
                userData[username] = hashed;
                QFile outFile("users.json");
                if (outFile.open(QIODevice::WriteOnly)) {
                    QJsonDocument outDoc(userData);
                    outFile.write(outDoc.toJson());
                    outFile.close();
                }
                authenticated = true;
            } else {
                if (!userData.contains(username) || userData[username].toString() != hashed) {
                    QMessageBox::warning(this, "Login Failed", "Invalid username or password.");
                    continue;
                }
                authenticated = true;
            }
            currentUser = username;
        } else {
            exit(0);
        }
    }
}

void MainWindow::setupUI() {
    QPixmap background("C:/Users/hp/Downloads/image/game_bg");
    // qDebug() << "Image loaded?" << !background.isNull();
    background = background.scaled(this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    QPalette palette;
    palette.setBrush(QPalette::Window, background);
    this->setAutoFillBackground(true);
    this->setPalette(palette);
    setFixedSize(1400, 900);


    QWidget *central = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(central);

    mainToolBar = addToolBar("Main Toolbar");
    historyAction = new QAction("Game History", this);
    newGameAction = new QAction("New Game", this);
    mainToolBar->addAction(historyAction);
    mainToolBar->addAction(newGameAction);
    logoutAction = new QAction("Logout", this);
    mainToolBar->addAction(logoutAction);
    connect(logoutAction, &QAction::triggered, this, &MainWindow::logout);

    connect(historyAction, &QAction::triggered, this, &MainWindow::showGameHistory);
    connect(newGameAction, &QAction::triggered, this, &MainWindow::startNewGame);

    QFont headerFont;
    headerFont.setPointSize(20);
    headerFont.setBold(true);

    statusLabel1 = new QLabel("Welcome to Tic Tac Toe", this);
    statusLabel1->setFont(headerFont);
    statusLabel1->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(statusLabel1);

    userLabel = new QLabel(QString("Logged in as: %1").arg(currentUser), this);
    userLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(userLabel);

    modeSelector = new QComboBox(this);
    modeSelector->addItem("Player vs Player");
    modeSelector->addItem("Player vs AI");
    connect(modeSelector, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::handleModeChange);
    mainLayout->addWidget(modeSelector);

    statusLabel = new QLabel("Player X's turn", this);
    statusLabel->setFont(headerFont);
    statusLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(statusLabel);

    QGridLayout *grid = new QGridLayout();
    QFont font;
    font.setPointSize(40);
    font.setBold(true);

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            buttons[i][j] = new QPushButton(" ", this);
            buttons[i][j]->setFixedSize(150, 150);
            buttons[i][j]->setFont(font);
            grid->addWidget(buttons[i][j], i, j);
            buttons[i][j]->setProperty("row", i);
            buttons[i][j]->setProperty("col", j);
            connect(buttons[i][j], &QPushButton::clicked, this, &MainWindow::handleButtonClick);
        }
    }
    mainLayout->addLayout(grid);

    setCentralWidget(central);
}

void MainWindow::resetGame() {
    board = Board();
    currentPlayer = 'X';
    gameOver = false;
    moveHistory.clear();

    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j) {
            buttons[i][j]->setText(" ");
            buttons[i][j]->setEnabled(true);
            buttons[i][j]->setStyleSheet("");
        }
    updateStatus();
}

void MainWindow::handleModeChange(int index) {
    mode = (index == 0) ? 1 : 2;
    resetGame();
}
void MainWindow::handleHistoryClick(QListWidgetItem *item) {
    // You can keep this empty or show a message for now
    QMessageBox::information(this, "History Item Clicked", item->text());
}


void MainWindow::handleButtonClick() {
    if (gameOver) return;

    QPushButton *btn = qobject_cast<QPushButton *>(sender());
    int row = btn->property("row").toInt();
    int col = btn->property("col").toInt();

    if (!board.makeMove(row, col, currentPlayer)) return;
    updateButton(row, col);
    moveHistory.append(QString("%1%2%3").arg(currentPlayer).arg(row).arg(col));

    if (board.checkWin(currentPlayer)) {
        statusLabel->setText(QString("Player %1 wins!").arg(currentPlayer));
        QMessageBox::information(this, "Game Over", QString("Player %1 wins!").arg(currentPlayer));
        saveGameRecord(QString(currentPlayer));
        gameOver = true;
        disableBoard();
        return;
    } else if (board.checkTie()) {
        statusLabel->setText("It's a tie!");
        QMessageBox::information(this, "Game Over", "It's a tie!");
        saveGameRecord("Tie");
        gameOver = true;
        disableBoard();
        return;
    }

    currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
    updateStatus();

    if (mode == 2 && currentPlayer == 'O') {
        makeAIMove();
    }
}

void MainWindow::updateButton(int row, int col) {
    QPushButton* btn = buttons[row][col];
    btn->setText(QString(currentPlayer));
    QString color = (currentPlayer == 'X') ? "color: red" : "color: blue";
    btn->setStyleSheet(color);
}

void MainWindow::makeAIMove() {
    auto [row, col] = ai.getBestMove(board, 'O', 'X');
    board.makeMove(row, col, 'O');
    updateButton(row, col);
    moveHistory.append(QString("O%1%2").arg(row).arg(col));

    if (board.checkWin('O')) {
        statusLabel->setText("Computer wins!");
        QMessageBox::information(this, "Game Over", "Computer wins!");
        saveGameRecord("O");
        gameOver = true;
        disableBoard();
        return;
    } else if (board.checkTie()) {
        statusLabel->setText("It's a tie!");
        QMessageBox::information(this, "Game Over", "It's a tie!");
        saveGameRecord("Tie");
        gameOver = true;
        disableBoard();
        return;
    }

    currentPlayer = 'X';
    updateStatus();
}

void MainWindow::updateStatus() {
    if (!gameOver)
        statusLabel->setText(QString("Player %1's turn").arg(currentPlayer));
}

void MainWindow::disableBoard() {
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            buttons[i][j]->setEnabled(false);
}

void MainWindow::saveGameRecord(QString winner) {
    QFile file("history.json");
    QJsonObject root;
    if (file.open(QIODevice::ReadOnly)) {
        root = QJsonDocument::fromJson(file.readAll()).object();
        file.close();
    }

    QJsonArray games = root[currentUser].toArray();
    QJsonObject record;
    record["winner"] = winner;
    record["mode"] = (mode == 1) ? "PvP" : "PvAI";
    QJsonArray moves;
    for (const QString &m : moveHistory) moves.append(m);
    record["moves"] = moves;
    games.append(record);
    root[currentUser] = games;

    if (file.open(QIODevice::WriteOnly)) {
        file.write(QJsonDocument(root).toJson());
        file.close();
    }
}

void MainWindow::showGameHistory() {
    QDialog *historyDialog = new QDialog(this);
    historyDialog->setWindowTitle("Game History");
    QVBoxLayout *layout = new QVBoxLayout(historyDialog);

    QListWidget *list = new QListWidget(historyDialog);
    layout->addWidget(list);

    QFile file("history.json");
    QJsonObject root;
    if (file.open(QIODevice::ReadOnly)) {
        root = QJsonDocument::fromJson(file.readAll()).object();
        file.close();
    }

    QJsonArray games = root[currentUser].toArray();
    for (const auto &game : games) {
        QJsonObject g = game.toObject();
        list->addItem(QString("%1: %2").arg(g["mode"].toString()).arg(g["winner"].toString()));
    }

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *replayBtn = new QPushButton("Replay", historyDialog);
    QPushButton *deleteBtn = new QPushButton("Delete", historyDialog);
    buttonLayout->addWidget(replayBtn);
    buttonLayout->addWidget(deleteBtn);
    layout->addLayout(buttonLayout);

    connect(replayBtn, &QPushButton::clicked, this, [this, list, games]() {
        int index = list->currentRow();
        if (index >= 0 && index < games.size()) {
            QJsonObject g = games[index].toObject();
            QJsonArray moves = g["moves"].toArray();
            resetGame();
            for (const auto &m : moves) {
                QString move = m.toString();
                if (move.length() == 3) {
                    char player = move[0].toLatin1();
                    int r = move[1].digitValue();
                    int c = move[2].digitValue();
                    board.makeMove(r, c, player);
                    currentPlayer = player;
                    updateButton(r, c);
                }
            }
            gameOver = true;
            updateStatus();
        }
    });

    connect(deleteBtn, &QPushButton::clicked, this, [this, list, &root, historyDialog]() {
        int index = list->currentRow();
        if (index >= 0) {
            QJsonArray games = root[currentUser].toArray();
            games.removeAt(index);
            root[currentUser] = games;

            QFile file("history.json");
            if (file.open(QIODevice::WriteOnly)) {
                file.write(QJsonDocument(root).toJson());
                file.close();
            }
            list->takeItem(index);
            QMessageBox::information(historyDialog, "Deleted", "Game record deleted.");
        }
    });

    historyDialog->exec();
}

void MainWindow::startNewGame() {
    resetGame();
}



/*#include "mainwindow.h"
#include "LoginDialog.h"
#include <QVBoxLayout>
#include <QGridLayout>
#include <QMessageBox>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QCryptographicHash>
#include <QToolBar>
#include <QAction>
#include <QFont>
#include <QDialog>
#include <QListWidget>
#include <QPushButton>
#include <QHBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), currentPlayer('X'), mode(1), gameOver(false) {
    authenticateUser();
    setupUI();
    resetGame();
}
void MainWindow::handleHistoryClick(QListWidgetItem *item) {
    // You can keep this empty or show a message for now
    QMessageBox::information(this, "History Item Clicked", item->text());
}

void MainWindow::authenticateUser() {
    bool authenticated = false;
    while (!authenticated) {
        LoginDialog login(this);
        if (login.exec() == QDialog::Accepted) {
            QString username = login.getUsername();
            QString password = login.getPassword();
            QString hashed = QString(QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex());

            QFile file("users.json");
            QJsonObject userData;
            if (file.open(QIODevice::ReadOnly)) {
                QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
                userData = doc.object();
                file.close();
            }

            if (login.isRegistering()) {
                if (userData.contains(username)) {
                    QMessageBox::warning(this, "Register", "Username already exists!");
                    continue;
                }
                userData[username] = hashed;
                QFile outFile("users.json");
                if (outFile.open(QIODevice::WriteOnly)) {
                    QJsonDocument outDoc(userData);
                    outFile.write(outDoc.toJson());
                    outFile.close();
                }
                authenticated = true;
            } else {
                if (!userData.contains(username) || userData[username].toString() != hashed) {
                    QMessageBox::warning(this, "Login Failed", "Invalid username or password.");
                    continue;
                }
                authenticated = true;
            }
            currentUser = username;
        } else {
            exit(0);
        }
    }
}

void MainWindow::setupUI() {
    QPixmap background("C:/Users/hp/Downloads/image/A.PNG");
    // qDebug() << "Image loaded?" << !background.isNull();
    background = background.scaled(this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    QPalette palette;
    palette.setBrush(QPalette::Window, background);
    this->setAutoFillBackground(true);
    this->setPalette(palette);

    QWidget *central = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(central);

    mainToolBar = addToolBar("Main Toolbar");
    historyAction = new QAction("Game History", this);
    newGameAction = new QAction("New Game", this);
    mainToolBar->addAction(historyAction);
    mainToolBar->addAction(newGameAction);
    logoutAction = new QAction("Logout", this);
    mainToolBar->addAction(logoutAction);
    connect(logoutAction, &QAction::triggered, this, &MainWindow::logout);

    connect(historyAction, &QAction::triggered, this, &MainWindow::showGameHistory);
    connect(newGameAction, &QAction::triggered, this, &MainWindow::startNewGame);

    QFont headerFont;
    headerFont.setPointSize(20);
    headerFont.setBold(true);

    statusLabel1 = new QLabel("Welcome to Tic Tac Toe", this);
    statusLabel1->setFont(headerFont);
    statusLabel1->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(statusLabel1);

    userLabel = new QLabel(QString("Logged in as: %1").arg(currentUser), this);
    userLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(userLabel);

    modeSelector = new QComboBox(this);
    modeSelector->addItem("Player vs Player");
    modeSelector->addItem("Player vs AI");
    connect(modeSelector, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::handleModeChange);
    mainLayout->addWidget(modeSelector);

    statusLabel = new QLabel("Player X's turn", this);
    statusLabel->setFont(headerFont);
    statusLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(statusLabel);

    QGridLayout *grid = new QGridLayout();
    QFont font;
    font.setPointSize(40);
    font.setBold(true);

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            buttons[i][j] = new QPushButton(" ", this);
            buttons[i][j]->setFixedSize(150, 150);
            buttons[i][j]->setFont(font);
            grid->addWidget(buttons[i][j], i, j);
            buttons[i][j]->setProperty("row", i);
            buttons[i][j]->setProperty("col", j);
            connect(buttons[i][j], &QPushButton::clicked, this, &MainWindow::handleButtonClick);
        }
    }
    mainLayout->addLayout(grid);

    central->setStyleSheet("background: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 #ffecd2, stop:0.5 #fcb69f, stop:1 #f6d365);");
        setCentralWidget(central);
}

void MainWindow::resetGame() {
    board = Board();
    currentPlayer = 'X';
    gameOver = false;
    moveHistory.clear();

    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j) {
            buttons[i][j]->setText(" ");
            buttons[i][j]->setEnabled(true);
            buttons[i][j]->setStyleSheet("");
        }
    updateStatus();
}

void MainWindow::handleModeChange(int index) {
    mode = (index == 0) ? 1 : 2;
    resetGame();
}

void MainWindow::handleButtonClick() {
    if (gameOver) return;

    QPushButton *btn = qobject_cast<QPushButton *>(sender());
    int row = btn->property("row").toInt();
    int col = btn->property("col").toInt();

    if (!board.makeMove(row, col, currentPlayer)) return;
    updateButton(row, col);
    moveHistory.append(QString("%1%2%3").arg(currentPlayer).arg(row).arg(col));

    if (board.checkWin(currentPlayer)) {
        statusLabel->setText(QString("Player %1 wins!").arg(currentPlayer));
        QMessageBox::information(this, "Game Over", QString("Player %1 wins!").arg(currentPlayer));
        saveGameRecord(QString(currentPlayer));
        gameOver = true;
        disableBoard();
        return;
    } else if (board.checkTie()) {
        statusLabel->setText("It's a tie!");
        QMessageBox::information(this, "Game Over", "It's a tie!");
        saveGameRecord("Tie");
        gameOver = true;
        disableBoard();
        return;
    }

    currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
    updateStatus();

    if (mode == 2 && currentPlayer == 'O') {
        makeAIMove();
    }
}

void MainWindow::updateButton(int row, int col) {
    QPushButton* btn = buttons[row][col];
    btn->setText(QString(currentPlayer));
    QString color = (currentPlayer == 'X') ? "color: red" : "color: blue";
    btn->setStyleSheet(color);
}

void MainWindow::makeAIMove() {
    auto [row, col] = ai.getBestMove(board, 'O', 'X');
    board.makeMove(row, col, 'O');
    updateButton(row, col);
    moveHistory.append(QString("O%1%2").arg(row).arg(col));

    if (board.checkWin('O')) {
        statusLabel->setText("Computer wins!");
        QMessageBox::information(this, "Game Over", "Computer wins!");
        saveGameRecord("O");
        gameOver = true;
        disableBoard();
        return;
    } else if (board.checkTie()) {
        statusLabel->setText("It's a tie!");
        QMessageBox::information(this, "Game Over", "It's a tie!");
        saveGameRecord("Tie");
        gameOver = true;
        disableBoard();
        return;
    }

    currentPlayer = 'X';
    updateStatus();
}

void MainWindow::updateStatus() {
    if (!gameOver)
        statusLabel->setText(QString("Player %1's turn").arg(currentPlayer));
}

void MainWindow::disableBoard() {
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            buttons[i][j]->setEnabled(false);
}

void MainWindow::saveGameRecord(QString winner) {
    QFile file("history.json");
    QJsonObject root;
    if (file.open(QIODevice::ReadOnly)) {
        root = QJsonDocument::fromJson(file.readAll()).object();
        file.close();
    }

    QJsonArray games = root[currentUser].toArray();
    QJsonObject record;
    record["winner"] = winner;
    record["mode"] = (mode == 1) ? "PvP" : "PvAI";
    QJsonArray moves;
    for (const QString &m : moveHistory) moves.append(m);
    record["moves"] = moves;
    games.append(record);
    root[currentUser] = games;

    if (file.open(QIODevice::WriteOnly)) {
        file.write(QJsonDocument(root).toJson());
        file.close();
    }
}

void MainWindow::showGameHistory() {
    QDialog *historyDialog = new QDialog(this);
    historyDialog->setWindowTitle("Game History");
    QVBoxLayout *layout = new QVBoxLayout(historyDialog);

    QListWidget *list = new QListWidget(historyDialog);
    layout->addWidget(list);

    QFile file("history.json");
    QJsonObject root;
    if (file.open(QIODevice::ReadOnly)) {
        root = QJsonDocument::fromJson(file.readAll()).object();
        file.close();
    }

    QJsonArray games = root[currentUser].toArray();
    for (const auto &game : games) {
        QJsonObject g = game.toObject();
        list->addItem(QString("%1: %2").arg(g["mode"].toString()).arg(g["winner"].toString()));
    }

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *replayBtn = new QPushButton("Replay", historyDialog);
    QPushButton *deleteBtn = new QPushButton("Delete", historyDialog);
    buttonLayout->addWidget(replayBtn);
    buttonLayout->addWidget(deleteBtn);
    layout->addLayout(buttonLayout);

    connect(replayBtn, &QPushButton::clicked, this, [this, list, games]() {
        int index = list->currentRow();
        if (index >= 0 && index < games.size()) {
            QJsonObject g = games[index].toObject();
            QJsonArray moves = g["moves"].toArray();
            resetGame();
            for (const auto &m : moves) {
                QString move = m.toString();
                if (move.length() == 3) {
                    char player = move[0].toLatin1();
                    int r = move[1].digitValue();
                    int c = move[2].digitValue();
                    board.makeMove(r, c, player);
                    currentPlayer = player;
                    updateButton(r, c);
                }
            }
            gameOver = true;
            updateStatus();
        }
    });

    connect(deleteBtn, &QPushButton::clicked, this, [this, list, &root, historyDialog]() {
        int index = list->currentRow();
        if (index >= 0) {
            QJsonArray games = root[currentUser].toArray();
            games.removeAt(index);
            root[currentUser] = games;

            QFile file("history.json");
            if (file.open(QIODevice::WriteOnly)) {
                file.write(QJsonDocument(root).toJson());
                file.close();
            }
            list->takeItem(index);
            QMessageBox::information(historyDialog, "Deleted", "Game record deleted.");
        }
    });

    historyDialog->exec();
}

void MainWindow::startNewGame() {
    resetGame();
}
*/


void MainWindow::logout() {
    this->hide();
    LoginDialog loginDialog;
    if (loginDialog.exec() == QDialog::Accepted) {
        this->show();
    } else {
        QApplication::quit();
    }
}


void MainWindow::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event);

    QPixmap background("C:/Users/hp/Downloads/image/game_bg");
    background = background.scaled(this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    QPalette palette;
    palette.setBrush(QPalette::Window, background);
    this->setPalette(palette);
}
