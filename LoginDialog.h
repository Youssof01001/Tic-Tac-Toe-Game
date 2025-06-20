#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QStackedWidget>

class LoginDialog : public QDialog {
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    QString getUsername() const;
    QString getPassword() const;
    QString getPlayer2Username() const;
    int getSelectedMode() const;
    bool isRegistering() const;

private slots:
    void handleTwoPlayersClicked();
    void handlePlayerVsAIClicked();
    void handleSignInClicked();
    void handleNewPlayerClicked();
    void handleLoginClicked();
    void handleRegisterClicked();
    void handlePlayer2SignInClicked();
    void handlePlayer2NewPlayerClicked();
    void handlePlayer2LoginClicked();
    void handlePlayer2RegisterClicked();

private:
    QStackedWidget *stackedWidget;

    // Mode selection page
    QWidget *modeSelectionPage;
    QPushButton *twoPlayersButton;
    QPushButton *playerVsAIButton;

    // Auth type selection page
    QWidget *authTypeSelectionPage;
    QPushButton *signInButton;
    QPushButton *newPlayerButton;
    QPushButton *backToModeButton;

    // Login page
    QWidget *loginPage;
    QLineEdit *usernameEdit;
    QLineEdit *passwordEdit;
    QPushButton *loginButton;
    QPushButton *backToAuthButton;

    // Register page
    QWidget *registerPage;
    QLineEdit *regUsernameEdit;
    QLineEdit *regPasswordEdit;
    QLineEdit *regConfirmPasswordEdit;
    QPushButton *registerButton;
    QPushButton *backToAuthButton2;

    // Player 2 auth type selection (for two players mode)
    QWidget *player2AuthTypePage;
    QPushButton *player2SignInButton;
    QPushButton *player2NewPlayerButton;
    QLabel *player2InfoLabel;

    // Player 2 login page
    QWidget *player2LoginPage;
    QLineEdit *player2UsernameEdit;
    QLineEdit *player2PasswordEdit;
    QPushButton *player2LoginButton;
    QPushButton *backToPlayer2AuthButton;

    // Player 2 register page
    QWidget *player2RegisterPage;
    QLineEdit *player2RegUsernameEdit;
    QLineEdit *player2RegPasswordEdit;
    QLineEdit *player2RegConfirmPasswordEdit;
    QPushButton *player2RegisterButton;
    QPushButton *backToPlayer2AuthButton2;

    QLabel *statusLabel;

    QString currentUsername;
    QString currentPassword;
    QString player2Username;
    int selectedMode; // 1 = two players, 2 = player vs AI
    bool registering;

    void setupModeSelectionPage();
    void setupAuthTypeSelectionPage();
    void setupLoginPage();
    void setupRegisterPage();
    void setupPlayer2AuthTypePage();
    void setupPlayer2LoginPage();
    void setupPlayer2RegisterPage();
    void applyStyles();
};

#endif // LOGINDIALOG_H
