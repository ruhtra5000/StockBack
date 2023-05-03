#include "formpesquisarproduto.h"
#include "ui_formpesquisarproduto.h"

#include <QTableWidgetItem>
#include <QMessageBox>

FormPesquisarProduto::FormPesquisarProduto(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormPesquisarProduto)
{
    ui->setupUi(this);
    this->setWindowTitle("StockBack - Pesquisar produto");

    ui->selecionarButton->setFocus();

    //Configurando a tabela e settando os itens
    ui->tabelaSelecProdutos->setColumnCount(4);
    ui->tabelaSelecProdutos->setHorizontalHeaderLabels(QStringList {"Cód. Prod.", "Produto", "Estoque", "Preço"});
    ui->tabelaSelecProdutos->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tabelaSelecProdutos->verticalHeader()->setVisible(false);
    ui->tabelaSelecProdutos->setAlternatingRowColors(true);
    ui->tabelaSelecProdutos->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tabelaSelecProdutos->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tabelaSelecProdutos->setStyleSheet("QTableView {selection-background-color: #66696c}");

    QSqlQuery consulta;
    consulta.prepare("select id_produto,descricao,quantidade,valorVenda from Produtos");
    if(consulta.exec()){
        if(consulta.first()){
            int linha = 0;
            do {
                ui->tabelaSelecProdutos->insertRow(linha);
                ui->tabelaSelecProdutos->setItem(linha, 0, new QTableWidgetItem(consulta.value("id_produto").toString()));
                ui->tabelaSelecProdutos->setItem(linha, 1, new QTableWidgetItem(consulta.value("descricao").toString()));
                ui->tabelaSelecProdutos->setItem(linha, 2, new QTableWidgetItem(consulta.value("quantidade").toString()));
                ui->tabelaSelecProdutos->setItem(linha, 3, new QTableWidgetItem("R$ " + consulta.value("valorVenda").toString()));
                linha++;
            }
            while(consulta.next());
        }
    }
}

FormPesquisarProduto::~FormPesquisarProduto()
{
    delete ui;
}

void FormPesquisarProduto::on_selecionarButton_clicked(){
    if(ui->tabelaSelecProdutos->currentRow() == -1){
        QMessageBox::warning(this, "StockBack", "Selecione um produto!");
        return;
    }

    this->codProd = ui->tabelaSelecProdutos->item(ui->tabelaSelecProdutos->currentRow(), 0)->text().toInt();
    this->selec = true;
    this->close();
}

void FormPesquisarProduto::on_cancelarButton_clicked(){
    this->close();
}

