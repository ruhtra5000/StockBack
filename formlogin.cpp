#include "formlogin.h"
#include "ui_formlogin.h"

#include "formprincipal.h"

#include <QMessageBox>
#include <QGraphicsEffect>

FormLogin::FormLogin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormLogin)
{
    ui->setupUi(this);
    this->setWindowTitle("StockBack");
    this->setWindowState(Qt::WindowMaximized);

    QGraphicsDropShadowEffect *sombra = new QGraphicsDropShadowEffect();
    sombra->setBlurRadius(10);
    sombra->setOffset(3);
    ui->frameLogin->setGraphicsEffect(sombra);
}

FormLogin::~FormLogin()
{
    delete ui;
}

void FormLogin::on_loginButton_clicked(){
    if(ui->username->text() != "" && ui->senha->text() != ""){
        QSqlQuery consulta;
        consulta.prepare("select * from Funcionarios where username = ? and senha = ?");
        consulta.addBindValue(ui->username->text());
        consulta.addBindValue(ui->senha->text());
        if(consulta.exec()){
            if(consulta.first()){
                FormPrincipal::idFunc = consulta.value("id_funcionario").toInt();
                FormPrincipal::nomeFunc = consulta.value("nome").toString();
                FormPrincipal::acessoFunc = consulta.value("acesso").toString();
                this->close();
            }
            else {
                QMessageBox::warning(this, "StockBack - Login", "Usuário ou senha incorretos!");
            }
        }
    }
}

