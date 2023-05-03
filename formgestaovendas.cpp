#include "formgestaovendas.h"
#include "ui_formgestaovendas.h"

#include <QMessageBox>
#include <QTableWidgetItem>

FormGestaoVendas::FormGestaoVendas(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormGestaoVendas)
{
    ui->setupUi(this);
    this->setWindowTitle("StockBack - Gestão de vendas");

    //Stattus bar
    bar = new QStatusBar(this);
    ui->statusbarLayout->addWidget(bar);

    //Configurando as tabelas
    ui->tabelaVendas->setColumnCount(6);
    ui->tabelaVendas->setHorizontalHeaderLabels(QStringList {"ID Venda", "Data", "Hora", "Responsável", "Valor total", "Pagamento"});
    ui->tabelaVendas->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tabelaVendas->verticalHeader()->setVisible(false);
    ui->tabelaVendas->setAlternatingRowColors(true);
    ui->tabelaVendas->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tabelaVendas->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tabelaVendas->setStyleSheet("QTableView {selection-background-color: #66696c}");

    ui->tabelaProdutosVendas->setColumnCount(5);
    ui->tabelaProdutosVendas->setHorizontalHeaderLabels(QStringList {"ID Mov.", "Produto", "Qtde", "Valor Un.", "Valor total"});
    ui->tabelaProdutosVendas->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tabelaProdutosVendas->verticalHeader()->setVisible(false);
    ui->tabelaProdutosVendas->setAlternatingRowColors(true);
    ui->tabelaProdutosVendas->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tabelaProdutosVendas->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tabelaProdutosVendas->setStyleSheet("QTableView {selection-background-color: #66696c}");

    atualizarVendas("select Vendas.*, Funcionarios.nome from Vendas inner join Funcionarios on Vendas.id_funcionario = Funcionarios.id_funcionario where fechado = 1", "Houve um erro de conexão com o banco de dados!");
}

FormGestaoVendas::~FormGestaoVendas()
{
    delete ui;
}

void FormGestaoVendas::atualizarVendas(QString query, QString msgErro){
    ui->tabelaVendas->clearContents();
    ui->tabelaVendas->setRowCount(0);

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
                ui->tabelaVendas->insertRow(linha);
                ui->tabelaVendas->setItem(linha, 0, new QTableWidgetItem(consulta.value("id_venda").toString()));
                ui->tabelaVendas->setItem(linha, 1, new QTableWidgetItem(consulta.value("dataVenda").toDate().toString("dd/MM/yyyy")));
                ui->tabelaVendas->setItem(linha, 2, new QTableWidgetItem(consulta.value("horaVenda").toString()));
                ui->tabelaVendas->setItem(linha, 3, new QTableWidgetItem(consulta.value("nome").toString()));
                ui->tabelaVendas->setItem(linha, 4, new QTableWidgetItem("R$ " + consulta.value("valorTotal").toString()));
                ui->tabelaVendas->setItem(linha, 5, new QTableWidgetItem(consulta.value("id_tipo_pagamento").toString()));
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

void FormGestaoVendas::atualizarProdutosVendas(QString idVenda){
    ui->tabelaProdutosVendas->clearContents();
    ui->tabelaProdutosVendas->setRowCount(0);

    this->con.abrir();
    QSqlQuery consulta;
    consulta.prepare("select * from produtosVendas where id_venda=?");
    consulta.addBindValue(idVenda);
    if(consulta.exec()){
        int linha = 0;
        while(consulta.next()) {
            ui->tabelaProdutosVendas->insertRow(linha);
            ui->tabelaProdutosVendas->setItem(linha, 0, new QTableWidgetItem(consulta.value("id_movimentacao").toString()));
            ui->tabelaProdutosVendas->setItem(linha, 1, new QTableWidgetItem(consulta.value("descricao").toString()));
            ui->tabelaProdutosVendas->setItem(linha, 2, new QTableWidgetItem(consulta.value("quantidade").toString()));
            ui->tabelaProdutosVendas->setItem(linha, 3, new QTableWidgetItem("R$ " + consulta.value("valorUnidade").toString()));
            ui->tabelaProdutosVendas->setItem(linha, 4, new QTableWidgetItem("R$ " + consulta.value("valorTotal").toString()));
            linha++;
        }
    }
    else {
        bar->showMessage("Houve um erro de conexão com o banco de dados!", 3000);
        bar->setStyleSheet("color: red;");
    }
    this->con.fechar();
}

void FormGestaoVendas::on_tabelaVendas_itemSelectionChanged(){
    if(ui->tabelaVendas->currentRow() != -1){
        atualizarProdutosVendas(ui->tabelaVendas->item(ui->tabelaVendas->currentRow(), 0)->text());
    }
}

void FormGestaoVendas::on_filtrarButton_clicked(){
    atualizarVendas("select Vendas.*, Funcionarios.nome from Vendas inner join Funcionarios on Vendas.id_funcionario = Funcionarios.id_funcionario where "
                    "dataVenda between '" + ui->dataInicio->date().toString("yyyy-MM-dd") + "' and '" + ui->dataFim->date().toString("yyyy-MM-dd") + "' and fechado = 1", "Nenhuma venda encontrada!");
}

void FormGestaoVendas::on_restaurarButton_clicked(){
    atualizarVendas("select Vendas.*, Funcionarios.nome from Vendas inner join Funcionarios on Vendas.id_funcionario = Funcionarios.id_funcionario where fechado = 1", "Houve um erro de conexão com o banco de dados!");
}

