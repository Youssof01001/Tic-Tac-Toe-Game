#include "mainwindow.h"
#include "LoginDialog.h"
#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>
#include <QListWidget>
#include <QToolBar>
#include <QAction>
#include <QMessageBox>
#include <QCryptographicHash>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QDialog>
#include <QFont>

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
            currentUser = login.getUsername();
            mode = login.getSelectedMode();

            if (mode == 1) {
                // Two players mode
                player2User = login.getPlayer2Username();
            }

            authenticated = true;
        } else {
            exit(0);
        }
    }
}

void MainWindow::setupUI() {
    QWidget *central = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(central);

    // Apply beautiful background gradient
    central->setStyleSheet(
        "QWidget {"
        "    background: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, "
        "    stop:0 #ffecd2, stop:0.5 #fcb69f, stop:1 #f093fb);"
        "}"
        );

    mainToolBar = addToolBar("Main Toolbar");
    mainToolBar->setStyleSheet(
        "QToolBar {"
        "    background: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, "
        "    stop:0 #667eea, stop:1 #764ba2);"
        "    border: none;"
        "    spacing: 10px;"
        "}"
        "QAction {"
        "    color: white;"
        "    font-weight: bold;"
        "    padding: 8px 16px;"
        "    margin: 4px;"
        "}"
        );

    historyAction = new QAction("🎮 Game History", this);
    newGameAction = new QAction("🎯 New Game", this);
    logoutAction = new QAction("🚪 Logout", this); // New logout action

    mainToolBar->addAction(historyAction);
    mainToolBar->addAction(newGameAction);
    mainToolBar->addSeparator();
    mainToolBar->addAction(logoutAction);

    connect(historyAction, &QAction::triggered, this, &MainWindow::showGameHistory);
    connect(newGameAction, &QAction::triggered, this, &MainWindow::startNewGame);
    connect(logoutAction, &QAction::triggered, this, &MainWindow::logout);

    QFont headerFont;
    headerFont.setPointSize(24);
    headerFont.setBold(true);

    statusLabel1 = new QLabel("🎲 Welcome to Tic Tac Toe 🎲", this);
    statusLabel1->setFont(headerFont);
    statusLabel1->setAlignment(Qt::AlignCenter);
    statusLabel1->setStyleSheet(
        "color: #2c3e50;"
        "background: rgba(255, 255, 255, 0.8);"
        "border-radius: 15px;"
        "padding: 15px;"
        "margin: 10px;"
        );
    mainLayout->addWidget(statusLabel1);

    QString userText;
    if (mode == 1) {
        userText = QString("👥 Players: %1 vs %2").arg(currentUser).arg(player2User);
    } else {
        userText = QString("🤖 Player: %1 vs AI").arg(currentUser);
    }

    userLabel = new QLabel(userText, this);
    userLabel->setAlignment(Qt::AlignCenter);
    userLabel->setStyleSheet(
        "font-size: 16px;"
        "color: #34495e;"
        "background: rgba(255, 255, 255, 0.7);"
        "border-radius: 10px;"
        "padding: 10px;"
        "margin: 5px;"
        );
    mainLayout->addWidget(userLabel);

    QFont statusFont;
    statusFont.setPointSize(18);
    statusFont.setBold(true);

    statusLabel = new QLabel("Player X's turn", this);
    statusLabel->setFont(statusFont);
    statusLabel->setAlignment(Qt::AlignCenter);
    statusLabel->setStyleSheet(
        "color: #e74c3c;"
        "background: rgba(255, 255, 255, 0.9);"
        "border-radius: 12px;"
        "padding: 12px;"
        "margin: 10px;"
        );
    mainLayout->addWidget(statusLabel);

    QGridLayout *grid = new QGridLayout();
    grid->setSpacing(8);

    QFont buttonFont;
    buttonFont.setPointSize(36);
    buttonFont.setBold(true);

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            buttons[i][j] = new QPushButton(" ", this);
            buttons[i][j]->setFixedSize(120, 120);
            buttons[i][j]->setFont(buttonFont);
            buttons[i][j]->setStyleSheet(
                "QPushButton {"
                "    background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, "
                "    stop:0 #ffffff, stop:1 #f8f9fa);"
                "    border: 3px solid #3498db;"
                "    border-radius: 15px;"
                "    color: #2c3e50;"
                "}"
                "QPushButton:hover {"
                "    background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, "
                "    stop:0 #e3f2fd, stop:1 #bbdefb);"
                "    border: 3px solid #2196f3;"
                "}"
                "QPushButton:pressed {"
                "    background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, "
                "    stop:0 #1976d2, stop:1 #1565c0);"
                "    color: white;"
                "}"
                );

            grid->addWidget(buttons[i][j], i, j);
            buttons[i][j]->setProperty("row", i);
            buttons[i][j]->setProperty("col", j);
            connect(buttons[i][j], &QPushButton::clicked, this, &MainWindow::handleButtonClick);
        }
    }

    QWidget *gridWidget = new QWidget();
    gridWidget->setLayout(grid);
    gridWidget->setStyleSheet(
        "QWidget {"
        "    background: rgba(255, 255, 255, 0.1);"
        "    border-radius: 20px;"
        "    padding: 20px;"
        "    margin: 10px;"
        "}"
        );

    mainLayout->addWidget(gridWidget);
    setCentralWidget(central);
}

void MainWindow::logout() {
    // Ask for confirmation
    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        "Logout Confirmation",
        "Are you sure you want to logout? Your current game will be lost.",
        QMessageBox::Yes | QMessageBox::No
        );

    if (reply == QMessageBox::Yes) {
        // Clear current user data
        currentUser.clear();
        player2User.clear();

        // Hide main window
        this->hide();

        // Show login dialog again
        authenticateUser();

        // Update UI with new user data
        setupUI();
        resetGame();

        // Show main window again
        this->show();
    }
}

void MainWindow::resetGame() {
    board = Board();
    currentPlayer = 'X';
    gameOver = false;
    moveHistory.clear();

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            buttons[i][j]->setText(" ");
            buttons[i][j]->setEnabled(true);
            buttons[i][j]->setStyleSheet(
                "QPushButton {"
                "    background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, "
                "    stop:0 #ffffff, stop:1 #f8f9fa);"
                "    border: 3px solid #3498db;"
                "    border-radius: 15px;"
                "    color: #2c3e50;"
                "}"
                "QPushButton:hover {"
                "    background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, "
                "    stop:0 #e3f2fd, stop:1 #bbdefb);"
                "    border: 3px solid #2196f3;"
                "}"
                "QPushButton:pressed {"
                "    background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, "
                "    stop:0 #1976d2, stop:1 #1565c0);"
                "    color: white;"
                "}"
                );
        }
    }

    updateStatus();
}

void MainWindow::handleModeChange(int index) {
    // This method is no longer used since mode is selected during login
}

void MainWindow::handleHistoryClick(QListWidgetItem *item) {
    QMessageBox::information(this, "History Item Clicked", item->text());
}

void MainWindow::handleButtonClick() {
    if (gameOver) return;

    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    int row = btn->property("row").toInt();
    int col = btn->property("col").toInt();

    if (!board.makeMove(row, col, currentPlayer)) return;

    updateButton(row, col);
    moveHistory.append(QString("%1%2%3").arg(currentPlayer).arg(row).arg(col));

    if (board.checkWin(currentPlayer)) {
        QString winnerName;
        if (mode == 1) {
            winnerName = (currentPlayer == 'X') ? currentUser : player2User;
        } else {
            winnerName = (currentPlayer == 'X') ? currentUser : "Computer";
        }

        statusLabel->setText(QString("🎉 %1 wins! 🎉").arg(winnerName));
        QMessageBox::information(this, "Game Over", QString("%1 wins!").arg(winnerName));
        saveGameRecord(QString(currentPlayer));
        gameOver = true;
        disableBoard();
        return;
    } else if (board.checkTie()) {
        statusLabel->setText("🤝 It's a tie! 🤝");
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

    QString color = (currentPlayer == 'X') ?
                        "color: #e74c3c; background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #ffebee, stop:1 #ffcdd2);" :
                        "color: #3498db; background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #e3f2fd, stop:1 #bbdefb);";

    btn->setStyleSheet(
        "QPushButton {"
        + color +
        "    border: 3px solid #2c3e50;"
        "    border-radius: 15px;"
        "    font-weight: bold;"
        "}"
        );
}

void MainWindow::makeAIMove() {
    auto [row, col] = ai.getBestMove(board, 'O', 'X');
    board.makeMove(row, col, 'O');
    updateButton(row, col);
    moveHistory.append(QString("O%1%2").arg(row).arg(col));

    if (board.checkWin('O')) {
        statusLabel->setText("🤖 Computer wins! 🤖");
        QMessageBox::information(this, "Game Over", "Computer wins!");
        saveGameRecord("O");
        gameOver = true;
        disableBoard();
        return;
    } else if (board.checkTie()) {
        statusLabel->setText("🤝 It's a tie! 🤝");
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
    if (!gameOver) {
        QString playerName;
        if (mode == 1) {
            playerName = (currentPlayer == 'X') ? currentUser : player2User;
        } else {
            playerName = (currentPlayer == 'X') ? currentUser : "Computer";
        }

        QString emoji = (currentPlayer == 'X') ? "❌" : "⭕";
        statusLabel->setText(QString("%1 %2's turn").arg(emoji).arg(playerName));
    }
}

void MainWindow::disableBoard() {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            buttons[i][j]->setEnabled(false);
        }
    }
}

void MainWindow::saveGameRecord(QString winner) {
    QFile file("history.json");
    QJsonObject root;
    if (file.open(QIODevice::ReadOnly)) {
        root = QJsonDocument::fromJson(file.readAll()).object();
        file.close();
    }

    // Save for player 1
    QJsonArray games = root[currentUser].toArray();
    QJsonObject record;
    record["winner"] = winner;
    record["mode"] = (mode == 1) ? "PvP" : "PvAI";
    if (mode == 1) {
        record["opponent"] = player2User;
    }
    QJsonArray moves;
    for (const QString &m : moveHistory) moves.append(m);
    record["moves"] = moves;
    games.append(record);
    root[currentUser] = games;

    // Save for player 2 in two-player mode
    if (mode == 1) {
        QJsonArray player2Games = root[player2User].toArray();
        QJsonObject player2Record = record;
        player2Record["opponent"] = currentUser;
        player2Games.append(player2Record);
        root[player2User] = player2Games;
    }

    if (file.open(QIODevice::WriteOnly)) {
        file.write(QJsonDocument(root).toJson());
        file.close();
    }
}

void MainWindow::showGameHistory() {
    QDialog *historyDialog = new QDialog(this);
    historyDialog->setWindowTitle("🎮 Game History");
    historyDialog->setFixedSize(600, 500);
    historyDialog->setStyleSheet(
        "QDialog {"
        "    background: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, "
        "    stop:0 #667eea, stop:1 #764ba2);"
        "}"
        "QListWidget {"
        "    background: rgba(255, 255, 255, 0.9);"
        "    border-radius: 10px;"
        "    padding: 10px;"
        "    font-size: 14px;"
        "    color: #2c3e50;"
        "}"
        "QPushButton {"
        "    background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, "
        "    stop:0 #4facfe, stop:1 #00f2fe);"
        "    border: none;"
        "    border-radius: 8px;"
        "    color: white;"
        "    font-weight: bold;"
        "    padding: 10px 20px;"
        "    margin: 5px;"
        "}"
        "QPushButton:hover {"
        "    background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, "
        "    stop:0 #43e97b, stop:1 #38f9d7);"
        "}"
        );

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
        QString opponent = g.contains("opponent") ? g["opponent"].toString() : "AI";
        list->addItem(QString("%1 vs %2: %3").arg(g["mode"].toString()).arg(opponent).arg(g["winner"].toString()));
    }

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *replayBtn = new QPushButton("🔄 Replay");
    QPushButton *deleteBtn = new QPushButton("🗑️ Delete");
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
