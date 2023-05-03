#include "formgestaoestoque.h"
#include "ui_formgestaoestoque.h"

#include "formeditarproduto.h"

#include <QMessageBox>
#include <QRegularExpression>

FormGestaoEstoque::FormGestaoEstoque(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormGestaoEstoque)
{
    ui->setupUi(this);
    this->setWindowTitle("StockBack - Gestão de estoque");
    ui->tabEstoque->setCurrentIndex(0);

    //Status bar
    bar = new QStatusBar(this);
    ui->statusbarLayout->addWidget(bar);

    //Settando tabela de produtos
    ui->tabelaProdutos->setColumnCount(5);
    ui->tabelaProdutos->setHorizontalHeaderLabels(QStringList{"Cód. Prod.", "Descrição", "Qtde", "Valor compra", "Valor venda"});
    ui->tabelaProdutos->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tabelaProdutos->verticalHeader()->setVisible(false);
    ui->tabelaProdutos->setAlternatingRowColors(true);
    ui->tabelaProdutos->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tabelaProdutos->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tabelaProdutos->setStyleSheet("QTableView {selection-background-color: #66696c}");

    atualizarProdutos("select * from Produtos", "Houve um erro de conexão com o banco de dados!");

    //Settando o codigo do novo produto
    setCodigoProduto();
}

FormGestaoEstoque::~FormGestaoEstoque()
{
    delete ui;
}

//Estoque - GESTÃO
void FormGestaoEstoque::atualizarProdutos(QString query, QString msgErro){
    ui->tabelaProdutos->clearContents();
    ui->tabelaProdutos->setRowCount(0);

    this->con.abrir();

    QSqlQuery consulta;
    consulta.prepare(query);
    if(consulta.exec()){
        if(!consulta.first()){
            bar->showMessage(msgErro, 3000);
            bar->setStyleSheet("color: red;");
        }
        else {
            int linha = 0;
            do {
                ui->tabelaProdutos->insertRow(linha);
                ui->tabelaProdutos->setItem(linha, 0, new QTableWidgetItem(consulta.value("id_produto").toString()));
                ui->tabelaProdutos->setItem(linha, 1, new QTableWidgetItem(consulta.value("descricao").toString()));
                ui->tabelaProdutos->setItem(linha, 2, new QTableWidgetItem(consulta.value("quantidade").toString()));
                ui->tabelaProdutos->setItem(linha, 3, new QTableWidgetItem("R$ " + consulta.value("valorCompra").toString()));
                ui->tabelaProdutos->setItem(linha, 4, new QTableWidgetItem("R$ " + consulta.value("valorVenda").toString()));
                linha++;
            }
            while(consulta.next());
        }
    }
    else {
        bar->showMessage(msgErro, 3000);
        bar->setStyleSheet("color: red;");
    }
    this->con.fechar();
}

void FormGestaoEstoque::on_editarButton_clicked(){
    this->con.abrir();
    if(ui->tabelaProdutos->currentRow() != -1){
        int linha = ui->tabelaProdutos->currentRow();
        FormEditarProduto *FEP = new FormEditarProduto(this, ui->tabelaProdutos->item(linha, 0)->text());
        FEP->exec();
        if(FEP->editado){
            this->atualizarProdutos("select * from Produtos", "Houve um erro de conexão com o banco de dados!");
            bar->showMessage("Produto editado com sucesso!", 3000);
            bar->setStyleSheet("color: green;");
        }
    }
    else {
        bar->showMessage("Nenhum item selecionado!", 3000);
        bar->setStyleSheet("color: red;");
    }
    this->con.fechar();
}

void FormGestaoEstoque::on_deletarButton_clicked(){
    if(ui->tabelaProdutos->currentRow() != -1){
        int linha = ui->tabelaProdutos->currentRow();
        QMessageBox::StandardButton res = QMessageBox::question(this, "StockBack", "Deseja deletar o produto " + ui->tabelaProdutos->item(linha, 1)->text() + "?", QMessageBox::Yes|QMessageBox::No);
        if(res == QMessageBox::Yes){
            this->con.abrir();
            QSqlQuery deletar;
            deletar.prepare("delete from Produtos where id_produto=?");
            deletar.addBindValue(ui->tabelaProdutos->item(linha, 0)->text());
            if(deletar.exec()){
                ui->tabelaProdutos->removeRow(linha);
                bar->showMessage("Produto deletado!");
                bar->setStyleSheet("color: green;");
            }
            this->con.fechar();
        }
    }
    else {
        bar->showMessage("Nenhum item selecionado!", 3000);
        bar->setStyleSheet("color: red;");
    }
}

void FormGestaoEstoque::on_filtrarButton_clicked(){
    if(ui->codigoRadioButton->isChecked()){
        if(ui->busca->text() == ""){
            atualizarProdutos("select * from Produtos", "Houve um erro de conexão com o banco de dados!");
        }
        else {
            QRegularExpression regex ("[^0-9]");
            QRegularExpressionMatch tem = regex.match(ui->busca->text());
            if(tem.hasMatch()){
                bar->showMessage("O código deve conter apenas números!", 3000);
                bar->setStyleSheet("color: red;");
            }
            else {
                atualizarProdutos("select * from Produtos where id_produto=" + ui->busca->text(), "Nenhum produto encontrado!");
                ui->busca->clear();
            }
        }
    }
    else if(ui->descRadioButton->isChecked()){
        if(ui->busca->text() == ""){
            atualizarProdutos("select * from Produtos order by descricao", "Houve um erro de conexão com o banco de dados!");
        }
        else {
            atualizarProdutos("select * from Produtos where descricao like '%" + ui->busca->text() + "%' order by descricao", "Nenhum produto encontrado!");
            ui->busca->clear();
        }
    }
}

void FormGestaoEstoque::on_codigoRadioButton_clicked(){
    ui->busca->setPlaceholderText("Código");
}

void FormGestaoEstoque::on_descRadioButton_clicked(){
    ui->busca->setPlaceholderText("Descrição");
}

//Estoque - ADICIONAR PRODUTO
void FormGestaoEstoque::on_criarButton_clicked(){
    if(ui->descricao->text() == "" || ui->valorCompra->value() == 0 || ui->valorVenda->value() == 0) {
        bar->showMessage("Preencha todos os campos!", 3000);
        bar->setStyleSheet("color: red;");
    }
    else {
        this->con.abrir();
        QSqlQuery inserir;
        inserir.prepare("insert into Produtos (descricao,quantidade,valorCompra,valorVenda) values (?, ?, ?, ?)");
        inserir.addBindValue(ui->descricao->text());
        inserir.addBindValue(QString::number(ui->quantidade->value()));
        inserir.addBindValue(QString::number(ui->valorCompra->value()));
        inserir.addBindValue(QString::number(ui->valorVenda->value()));
        if(inserir.exec()){
            setCodigoProduto();
            on_limparButton_clicked();
            atualizarProdutos("select * from Produtos", "Houve um erro de conexão com o banco de dados!");
            bar->showMessage("Produto criado com sucesso!", 3000);
            bar->setStyleSheet("color: green;");
        }
        this->con.fechar();
    }
}

void FormGestaoEstoque::on_limparButton_clicked(){
    ui->descricao->clear();
    ui->quantidade->setValue(0);
    ui->valorCompra->setValue(0.00);
    ui->valorVenda->setValue(0.00);
}

void FormGestaoEstoque::setCodigoProduto(){
    this->con.abrir();
    QSqlQuery consulta;
    consulta.prepare("select id_produto from Produtos order by id_produto desc limit 1");
    if(consulta.exec()){
        if(consulta.first()){
            ui->codProd->setText(QString::number(consulta.value("id_produto").toInt() + 1));
        }
        else {
            ui->codProd->setText("1");
        }
    }
    this->con.fechar();
}

