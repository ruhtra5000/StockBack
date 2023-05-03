#include "formeditarprodutovenda.h"
#include "ui_formeditarprodutovenda.h"

#include <QMessageBox>

FormEditarProdutoVenda::FormEditarProdutoVenda(QWidget *parent, QString codProd, QString descricao,
                                                int quant, double valor) :
    QDialog(parent),
    ui(new Ui::FormEditarProdutoVenda)
{
    ui->setupUi(this);
    this->setWindowTitle("StockBack - Editar produto (Venda)");

    //Pegando os valores
    this->codProd = codProd;
    this->descricao = descricao;
    this->quant = quant;
    this->valor = valor;

    //Colocando nos campos
    ui->codProd->setText(this->codProd);
    ui->descricao->setText(this->descricao);
    ui->quantidade->setValue(this->quant);
    ui->valor->setValue(this->valor);
}

FormEditarProdutoVenda::~FormEditarProdutoVenda()
{
    delete ui;
}

void FormEditarProdutoVenda::on_confirmarButton_clicked(){
    if(ui->quantidade->value() != 0 && ui->valor->value() != 0){
        QSqlQuery consulta;
        consulta.prepare("select quantidade from Produtos where id_produto=?");
        consulta.addBindValue(this->codProd);
        if(consulta.exec()){
            if(consulta.first()){
                if(ui->quantidade->value() > consulta.value("quantidade").toInt()){
                    QMessageBox::warning(this, "StockBack", "Há apenas " + consulta.value("quantidade").toString() + " unidades deste produto em estoque!");
                }
                else {
                    this->quant = ui->quantidade->value();
                    this->valor = ui->valor->value();
                    this->editado = true;
                    this->close();
                }
            }
            else {
                QMessageBox::critical(this, "ERRO", "O produto não pôde ser editado. Remova-o e adicione-o à venda novamente.");
                this->close();
                return;
            }
        }
    }
}

void FormEditarProdutoVenda::on_cancelarButton_clicked(){
    QMessageBox::StandardButton res = QMessageBox::question(this, "StockBack", "Deseja cancelar a edição? Os dados informados serão perdidos.", QMessageBox::Yes|QMessageBox::No);
    if(res == QMessageBox::Yes){
        this->close();
    }
}

