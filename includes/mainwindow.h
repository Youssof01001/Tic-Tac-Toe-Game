#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QToolBar>
#include <QAction>
#include <QMessageBox>
#include <QDialog>
#include <QListWidget>
#include <QListWidgetItem>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QTimer>
#include <QPixmap>
#include <QPalette>
#include <QResizeEvent>
#include <QApplication>
#include <QPropertyAnimation>
#include <QRect>
#include <QCryptographicHash>
#include <QKeyEvent>
#include "Board.h"
#include "AIPlayer.h"
#include <QScrollArea>
#include <QFrame>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() = default;

protected:
    void resizeEvent(QResizeEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;

private slots:
    // Login slots
    void onModeChanged();
    void onSignInClicked();
    void onNewPlayerClicked();
    void onNextPlayerClicked();
    void onStartGameClicked();
    void onBackClicked();
    void onExitClicked();
    void toggleFullScreen();

    // Game slots
    void onGameButtonClicked();
    void onNewGameClicked();
    void onShowHistoryClicked();
    void onLogoutClicked();
    void makeAIMove();
    void onDifficultyChanged(int index);

private:
    // UI Setup methods
    void setupUI();
    void setupLoginView();
    void setupGameView();
    void setupStyling();
    void setBackgroundImage();
    void updateLayoutForMode();

    // Login methods
    void resetToModeSelection();
    void showPlayer1Auth();
    void showPlayer2Auth();
    void showGameStart();
    bool authenticateUser(const QString& username, const QString& password);
    bool registerUser(const QString& username, const QString& password);
    QString hashPassword(const QString& password);
    void switchToGameView();

    // Game methods
    void setupGameGrid();
    void setupToolbar();
    void resetGame();
    void updateGameStatus();
    void checkGameEnd();
    void saveGameHistory(const QString& winner);
    void showGameOverDialog(const QString& result);
    void animateButton(QPushButton* button);
    void showGameHistoryDialog();
    void replayGame(const QJsonObject& gameData);
    void deleteGameFromHistory(int gameIndex);

    // Main UI Components
    QStackedWidget* stackedWidget;
    QWidget* loginWidget;
    QWidget* gameWidget;

    // Login UI Components
    QVBoxLayout* loginMainLayout;
    QLabel* loginTitleLabel;
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
    QPushButton* exitButton;
    QPushButton* fullScreenToggleButton;

    // Game UI Components
    QWidget* leftPanel;
    QWidget* rightPanel;
    QHBoxLayout* gameMainLayout;
    QLabel* titleLabel;
    QLabel* statusLabel;
    QLabel* playersLabel;
    QGridLayout* gameGridLayout;
    QWidget* gameGridWidget;
    QPushButton* gameButtons[3][3];
    QToolBar* toolBar;
    QAction* newGameAction;
    QAction* historyAction;
    QAction* logoutAction;

    // AI Difficulty System
    QLabel* difficultyLabel;
    QComboBox* difficultyComboBox;


    // Game Logic
    Board* board;
    AIPlayer* aiPlayer;
    QString currentPlayer;
    QString player1Name;
    QString player2Name;
    QString gameMode;
    bool gameActive;
    QStringList moveHistory;
    QTimer* aiTimer;

    // Login Logic
    int currentStep;
    bool isFullScreen;
};

#endif // MAINWINDOW_H
