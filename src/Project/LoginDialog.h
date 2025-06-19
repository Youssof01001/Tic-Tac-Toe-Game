#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>

class LoginDialog : public QDialog {
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    QString getUsername() const;
    QString getPassword() const;
    bool isRegistering() const;
                                                  void resizeEvent(QResizeEvent* event) override;


private slots:
    void handleLoginClicked();
    void handleRegisterClicked();

private:
    QLineEdit *usernameEdit;
    QLineEdit *passwordEdit;
    QPushButton *loginButton;
    QPushButton *registerButton;
    QLabel *statusLabel;
    QString currentUsername;
    QString currentPassword;
    bool registering;
};

#endif // LOGINDIALOG_H
