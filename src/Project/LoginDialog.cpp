#include "LoginDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent), registering(false) {
    setWindowTitle("Login / Register");
    resize(400, 200);

    QVBoxLayout *layout = new QVBoxLayout(this);

    usernameEdit = new QLineEdit(this);
    usernameEdit->setPlaceholderText("Username");
    layout->addWidget(usernameEdit);

    passwordEdit = new QLineEdit(this);
    passwordEdit->setPlaceholderText("Password");
    passwordEdit->setEchoMode(QLineEdit::Password);
    layout->addWidget(passwordEdit);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    loginButton = new QPushButton("Login", this);
    registerButton = new QPushButton("Register", this);
    buttonLayout->addWidget(loginButton);
    buttonLayout->addWidget(registerButton);
    layout->addLayout(buttonLayout);

    statusLabel = new QLabel(this);
    layout->addWidget(statusLabel);

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
