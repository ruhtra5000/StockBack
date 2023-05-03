#include "formeditarproduto.h"
#include "ui_formeditarproduto.h"

#include <QMessageBox>

FormEditarProduto::FormEditarProduto(QWidget *parent, QString idProduto) :
    QDialog(parent),
    ui(new Ui::FormEditarProduto)
{
    ui->setupUi(this);
    this->setWindowTitle("StockBack - Editar produto");

    this->idProd = idProduto;

    //Settando os dados nos campos
    QSqlQuery consulta;
    consulta.prepare("select * from Produtos where id_produto=?");
    consulta.addBindValue(this->idProd);
    if(consulta.exec()){
        if(consulta.first()){
            ui->codProd->setText(consulta.value("id_produto").toString());
            ui->descricao->setText(consulta.value("descricao").toString());
            ui->quantidade->setValue(consulta.value("quantidade").toInt());
            ui->valorCompra->setValue(consulta.value("valorCompra").toDouble());
            ui->valorVenda->setValue(consulta.value("valorVenda").toDouble());
        }
        else {
            QMessageBox::critical(this,"ERRO", "O produto não foi encontrado no banco de dados! Por favor, reinicie o programa.");
            this->close();
            return;
        }
    }
}

FormEditarProduto::~FormEditarProduto()
{
    delete ui;
}

void FormEditarProduto::on_confirmarButton_clicked(){
    if(ui->descricao->text() == "" || ui->valorCompra->value() == 0 || ui->valorVenda->value() == 0) {
        QMessageBox::warning(this, "StockBack - Editar produto", "Preencha todos os campos!");
    }
    else {
        QSqlQuery atualizar;
        atualizar.prepare("update Produtos set descricao=?,quantidade=?,valorCompra=?,valorVenda=? where id_produto=?");
        atualizar.addBindValue(ui->descricao->text());
        atualizar.addBindValue(QString::number(ui->quantidade->value()));
        atualizar.addBindValue(QString::number(ui->valorCompra->value()));
        atualizar.addBindValue(QString::number(ui->valorVenda->value()));
        atualizar.addBindValue(this->idProd);
        if(atualizar.exec()){
            this->editado = true;
            this->close();
        }
    }
}

void FormEditarProduto::on_cancelarButton_clicked(){
    QMessageBox::StandardButton res = QMessageBox::question(this, "StockBack", "Deseja cancelar a edição? Os dados informados serão perdidos.", QMessageBox::Yes|QMessageBox::No);
    if(res == QMessageBox::Yes){
        this->close();
    }
}

