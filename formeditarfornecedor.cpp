#include "formeditarfornecedor.h"
#include "ui_formeditarfornecedor.h"

#include <QMessageBox>

FormEditarFornecedor::FormEditarFornecedor(QWidget *parent, QString idForn) :
    QDialog(parent),
    ui(new Ui::FormEditarFornecedor)
{
    ui->setupUi(this);
    this->setWindowTitle("StockBack - Editar fornecedor");

    this->codForn = idForn;

    QSqlQuery consulta;
    consulta.prepare("select * from Fornecedores where id_fornecedor=?");
    consulta.addBindValue(this->codForn);
    if(consulta.exec()){
        if(consulta.first()){
            ui->codForn->setText(consulta.value("id_fornecedor").toString());
            ui->nome->setText(consulta.value("nome").toString());
            ui->cnpj->setText(consulta.value("cnpj").toString());
            ui->telefone->setText(consulta.value("telefone").toString());
            ui->email->setText(consulta.value("email").toString());
        }
        else {
            QMessageBox::critical(this, "ERRO", "O fornecedor selecionado não foi encontrado! Por favor, reinicie o programa.");
            this->close();
            return;
        }
    }
}

FormEditarFornecedor::~FormEditarFornecedor()
{
    delete ui;
}

void FormEditarFornecedor::on_confirmarButton_clicked(){
    if(ui->nome->text() != "" && ui->cnpj->text() != "" && ui->telefone->text() != "" && ui->email->text() != ""){
        QSqlQuery atualizar;
        atualizar.prepare("update Fornecedores set nome=?, cnpj=?, telefone=?, email=? where id_fornecedor=?");
        atualizar.addBindValue(ui->nome->text());
        atualizar.addBindValue(ui->cnpj->text());
        atualizar.addBindValue(ui->telefone->text());
        atualizar.addBindValue(ui->email->text());
        atualizar.addBindValue(this->codForn);
        if(atualizar.exec()){
            this->editado = true;
            this->close();
            return;
        }
    }
    else {
        QMessageBox::warning(this, "StockBack", "Preencha todos os campos!");
        return;
    }
}

void FormEditarFornecedor::on_cancelarButton_clicked(){
    QMessageBox::StandardButton res = QMessageBox::question(this, "StockBack", "Deseja cancelar as alterações? Os dados inseridos serão perdidos!", QMessageBox::Yes|QMessageBox::No);
    if(res == QMessageBox::Yes){
        this->close();
    }
}

