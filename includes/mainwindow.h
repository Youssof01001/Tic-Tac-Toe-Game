#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>
#include <QListWidget>
#include <QToolBar>
#include <QAction>
#include "Board.h"
#include "AIPlayer.h"
#include "LoginDialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void handleButtonClick();
    void handleModeChange(int index);
    void resetGame();
    void handleHistoryClick(QListWidgetItem *item);
    void showGameHistory();
    void startNewGame();
    void logout(); // New logout slot

private:
    Ui::MainWindow *ui;
    QPushButton* buttons[3][3];
    QComboBox* modeSelector;
    QLabel* statusLabel;
    QLabel* statusLabel1;
    QLabel* userLabel;
    QListWidget* historyList;
    QToolBar* mainToolBar;
    QAction* historyAction;
    QAction* newGameAction;
    QAction* logoutAction; // New logout action

    Board board;
    AIPlayer ai;
    char currentPlayer;
    int mode;
    bool gameOver;
    QString currentUser;
    QString player2User; // For two-player mode
    QVector<QString> moveHistory;

    void updateButton(int row, int col);
    void makeAIMove();
    void updateStatus();
    void setupUI();
    void disableBoard();
    void authenticateUser();
    void saveGameRecord(QString winner);
    void loadGameHistory();
};

#endif // MAINWINDOW_H
