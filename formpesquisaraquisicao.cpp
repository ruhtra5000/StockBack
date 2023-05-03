#include "formpesquisaraquisicao.h"
#include "ui_formpesquisaraquisicao.h"

#include <QMessageBox>

FormPesquisarAquisicao::FormPesquisarAquisicao(QWidget *parent, int tipo) :
    QDialog(parent),
    ui(new Ui::FormPesquisarAquisicao)
{
    ui->setupUi(this);

    //Configurações gerais da tabela
    ui->tabelaGeral->setColumnCount(3);
    ui->tabelaGeral->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tabelaGeral->verticalHeader()->setVisible(false);
    ui->tabelaGeral->setAlternatingRowColors(true);
    ui->tabelaGeral->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tabelaGeral->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tabelaGeral->setStyleSheet("QTableView {selection-background-color: #66696c}");

    //FORNECEDORES
    if(tipo == 1){
        this->setWindowTitle("StockBack - Pesquisar fornecedor");
        ui->tabelaGeral->setHorizontalHeaderLabels(this->forn);
        carregarDados("select id_fornecedor,nome,cnpj from Fornecedores", tipo);
    }

    //PRODUTOS
    else if(tipo == 2) {
        this->setWindowTitle("StockBack - Pesquisar produto");
        ui->tabelaGeral->setHorizontalHeaderLabels(this->prod);
        carregarDados("select id_produto,descricao,quantidade from Produtos", tipo);
    }
}

FormPesquisarAquisicao::~FormPesquisarAquisicao()
{
    delete ui;
}

void FormPesquisarAquisicao::carregarDados(QString query, int tipo){
    QSqlQuery consulta;
    consulta.prepare(query);
    if(consulta.exec()){
        if(!consulta.first()){
            QMessageBox::warning(this, "StockBack", "Nenhum dado foi encontrado!");
        }
        else {
            int linha = 0;
            if(tipo == 1){
                do {
                    ui->tabelaGeral->insertRow(linha);
                    ui->tabelaGeral->setItem(linha, 0, new QTableWidgetItem(consulta.value("id_fornecedor").toString()));
                    ui->tabelaGeral->setItem(linha, 1, new QTableWidgetItem(consulta.value("nome").toString()));
                    ui->tabelaGeral->setItem(linha, 2, new QTableWidgetItem(consulta.value("cnpj").toString()));
                    linha++;
                }
                while(consulta.next());
            }
            else {
                do {
                    ui->tabelaGeral->insertRow(linha);
                    ui->tabelaGeral->setItem(linha, 0, new QTableWidgetItem(consulta.value("id_produto").toString()));
                    ui->tabelaGeral->setItem(linha, 1, new QTableWidgetItem(consulta.value("descricao").toString()));
                    ui->tabelaGeral->setItem(linha, 2, new QTableWidgetItem(consulta.value("quantidade").toString()));
                    linha++;
                }
                while(consulta.next());
            }
        }
    }
    else {
        QMessageBox::warning(this, "StockBack", "Houve um erro ");
    }
}

void FormPesquisarAquisicao::on_selecionarButton_clicked(){
    if(ui->tabelaGeral->currentRow() != -1){
        this->codSelec = ui->tabelaGeral->item(ui->tabelaGeral->currentRow(), 0)->text();
        this->close();
    }
}

