#ifndef LOGIN_DIALOG_H
#define LOGIN_DIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    LoginDialog(QWidget *parent = nullptr);

private slots:
    void handleLogin();
    void handleCancel();

private:
    QLineEdit *lineEditAccount;
    QLineEdit *lineEditPassword;
    QLabel *labelError;
};

#endif // LOGIN_DIALOG_H
