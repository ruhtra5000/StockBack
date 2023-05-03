#include "formeditarfunc.h"
#include "ui_formeditarfunc.h"

#include <QMessageBox>

FormEditarFunc::FormEditarFunc(QWidget *parent, QString id_func) :
    QDialog(parent),
    ui(new Ui::FormEditarFunc)
{
    ui->setupUi(this);
    this->setWindowTitle("StockBack - Editar funcionário");

    this->idFunc = id_func;

    QSqlQuery consulta;
    consulta.prepare("select * from Funcionarios where id_funcionario=?");
    consulta.addBindValue(this->idFunc);
    if(consulta.exec()){
        if(consulta.first()){
            ui->idFunc->setText(consulta.value("id_funcionario").toString());
            ui->username->setText(consulta.value("username").toString());
            ui->senha->setText(consulta.value("senha").toString());
            ui->nome->setText(consulta.value("nome").toString());
            ui->telefone->setText(consulta.value("telefone").toString());
            ui->email->setText(consulta.value("email").toString());
            if(consulta.value("acesso").toString() == "A"){
                ui->acesso->setCurrentIndex(1);
            }

            this->usernamePadrao = consulta.value("username").toString();
        }
        else {
            QMessageBox::critical(this, "ERRO", "O funcionário selecionado não foi encontrado! Por favor, reinicie o programa.");
            this->close();
            return;
        }
    }
}

FormEditarFunc::~FormEditarFunc()
{
    delete ui;
}

void FormEditarFunc::on_confirmarButton_clicked(){
    if(ui->username->text() == "" || ui->senha->text() == "" || ui->nome->text() == "" || ui->telefone->text() == "" || ui->email->text() == ""){
        QMessageBox::warning(this, "StockBack", "Preencha todos os campos!");
    }
    else {
        QSqlQuery query;
        if(ui->username->text() != this->usernamePadrao){
            query.prepare("select username from Funcionarios where username=?");
            query.addBindValue(ui->username->text());
            if(query.exec()){
                if(query.first()){
                    QMessageBox::warning(this, "StockBack", "O username escolhido já pertence à outro usuário!");
                    return;
                }
            }
        }
        query.prepare("update Funcionarios set username=?, senha=?, nome=?, telefone=?, email=?, acesso=? where id_funcionario=?");
        query.addBindValue(ui->username->text());
        query.addBindValue(ui->senha->text());
        query.addBindValue(ui->nome->text());
        query.addBindValue(ui->telefone->text());
        query.addBindValue(ui->email->text());
        QString acesso = "B";
        if(ui->acesso->currentText() == "Admin"){
            acesso = "A";
        }
        query.addBindValue(acesso);
        query.addBindValue(this->idFunc);
        if(query.exec()){
            this->editado = true;
            this->close();
        }
    }
}

void FormEditarFunc::on_cancelarButton_clicked(){
    QMessageBox::StandardButton res = QMessageBox::question(this, "StockBack", "Deseja cancelar a edição? Os dados informados serão perdidos.", QMessageBox::Yes|QMessageBox::No);
    if(res == QMessageBox::Yes){
        this->close();
    }
}

