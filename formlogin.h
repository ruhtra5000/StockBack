#ifndef FORMLOGIN_H
#define FORMLOGIN_H

#include <QDialog>
#include <QtSql>

namespace Ui {
class FormLogin;
}

class FormLogin : public QDialog
{
    Q_OBJECT

public:
    explicit FormLogin(QWidget *parent = nullptr);
    ~FormLogin();

private slots:
    void on_loginButton_clicked();

private:
    Ui::FormLogin *ui;
};

#endif // FORMLOGIN_H
