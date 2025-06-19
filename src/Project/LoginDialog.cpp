#include "LoginDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPixmap>
#include <QPalette>
#include <QResizeEvent>
#include <QSpacerItem>
#include <QDebug>

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent), registering(false)
{
    setWindowTitle("Login / Register");
    resize(1400, 900);

    // Load and set background image
    QPixmap background("C:/Users/hp/Downloads/image/A.PNG");
    qDebug() << "Image loaded?" << !background.isNull();
    background = background.scaled(this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    QPalette palette;
    palette.setBrush(QPalette::Window, background);
    this->setAutoFillBackground(true);
    this->setPalette(palette);

    // --- Main vertical layout ---
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Add vertical spacer above
    mainLayout->addStretch();

    // Centered form layout
    QVBoxLayout *formLayout = new QVBoxLayout();
    formLayout->setContentsMargins(120, 0, 0, 0);  // Shift everything left

    usernameEdit = new QLineEdit(this);
    usernameEdit->setPlaceholderText("Username");
    usernameEdit->setFixedWidth(300);
    usernameEdit->setStyleSheet("padding: 10px; border-radius: 10px;");
    formLayout->addWidget(usernameEdit, 0, Qt::AlignLeft);

    passwordEdit = new QLineEdit(this);
    passwordEdit->setPlaceholderText("Password");
    passwordEdit->setEchoMode(QLineEdit::Password);
    passwordEdit->setFixedWidth(300);
    passwordEdit->setStyleSheet("padding: 10px; border-radius: 10px;");
    formLayout->addWidget(passwordEdit, 0, Qt::AlignLeft);

    // --- Vertical button layout ---
    QVBoxLayout *buttonLayout = new QVBoxLayout();
    buttonLayout->setContentsMargins(0, 10, 0, 0); // Small gap below password

    loginButton = new QPushButton("Login", this);
    registerButton = new QPushButton("Register", this);

    loginButton->setFixedWidth(100);
    registerButton->setFixedWidth(100);

    QString buttonStyle =
        "QPushButton {"
        "  background-color: #3498db;"
        "  color: white;"
        "  border: none;"
        "  border-radius: 10px;"
        "  padding: 10px;"
        "  font-size: 14px;"
        "}"
        "QPushButton:hover {"
        "  background-color: #5dade2;"
        "}"
        "QPushButton:pressed {"
        "  background-color: #2e86c1;"
        "}";

    loginButton->setStyleSheet(buttonStyle);
    registerButton->setStyleSheet(buttonStyle);

    buttonLayout->addWidget(loginButton, 0, Qt::AlignLeft);
    buttonLayout->addWidget(registerButton, 0, Qt::AlignLeft);

    formLayout->addLayout(buttonLayout);

    // Status label
    statusLabel = new QLabel(this);
    statusLabel->setStyleSheet("color: white;");
    formLayout->addWidget(statusLabel, 0, Qt::AlignLeft);

    // Add form to main layout
    mainLayout->addLayout(formLayout);

    // Add vertical spacer below
    mainLayout->addStretch();

    // Connect buttons
    connect(loginButton, &QPushButton::clicked, this, &LoginDialog::handleLoginClicked);
    connect(registerButton, &QPushButton::clicked, this, &LoginDialog::handleRegisterClicked);
}

QString LoginDialog::getUsername() const {
    return currentUsername;
}

QString LoginDialog::getPassword() const {
    return currentPassword;
}

bool LoginDialog::isRegistering() const {
    return registering;
}

void LoginDialog::handleLoginClicked() {
    registering = false;
    currentUsername = usernameEdit->text();
    currentPassword = passwordEdit->text();
    accept();
}

void LoginDialog::handleRegisterClicked() {
    registering = true;
    currentUsername = usernameEdit->text();
    currentPassword = passwordEdit->text();
    accept();
}

void LoginDialog::resizeEvent(QResizeEvent* event)
{
    QPixmap background("C:/Users/hp/Downloads/image/A.PNG");
    qDebug() << "Image loaded?" << !background.isNull();
    background = background.scaled(this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    QPalette palette;
    palette.setBrush(QPalette::Window, background);
    this->setAutoFillBackground(true);
    this->setPalette(palette);

    QDialog::resizeEvent(event);
}
