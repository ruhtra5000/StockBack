#include "formgestaofornecedores.h"
#include "ui_formgestaofornecedores.h"

#include "formeditarfornecedor.h"
#include "formnovaaquisicao.h"

#include <QMessageBox>
#include <QTableWidgetItem>
#include <QRegularExpression>

FormGestaoFornecedores::FormGestaoFornecedores(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormGestaoFornecedores)
{
    ui->setupUi(this);
    this->setWindowTitle("StockBack - Gestão de fornecedores");
    ui->tabFornecedores->setCurrentIndex(0);

    //Status bar
    bar = new QStatusBar();
    ui->statusbarLayout->addWidget(bar);

    //Configurando tabela de fornecedores
    ui->tabelaFornecedores->setColumnCount(5);
    ui->tabelaFornecedores->setHorizontalHeaderLabels(QStringList{"Cód. Fornecedor", "Nome", "CNPJ", "Telefone", "E-mail"});
    ui->tabelaFornecedores->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tabelaFornecedores->verticalHeader()->setVisible(false);
    ui->tabelaFornecedores->setAlternatingRowColors(true);
    ui->tabelaFornecedores->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tabelaFornecedores->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tabelaFornecedores->setStyleSheet("QTableView {selection-background-color: #66696c}");

    atualizarFornecedores("select * from Fornecedores", "Houve um erro de conexão com o banco de dados!");

    //Configurando a tabela de registros
    ui->tabelaRegistros->setColumnCount(6);
    ui->tabelaRegistros->setHorizontalHeaderLabels(QStringList{"ID Registro", "Data", "Hora", "Produto", "Quantidade", "Fornecedor"});
    ui->tabelaRegistros->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tabelaRegistros->verticalHeader()->setVisible(false);
    ui->tabelaRegistros->setAlternatingRowColors(true);
    ui->tabelaRegistros->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tabelaRegistros->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tabelaRegistros->setStyleSheet("QTableView {selection-background-color: #66696c}");

    atualizarRegistros("select Registros.*, Produtos.descricao, Fornecedores.nome from Registros "
                       "inner join Produtos on Produtos.id_produto = Registros.id_produto "
                       "inner join Fornecedores on Fornecedores.id_fornecedor = Registros.id_fornecedor",
                       "Houve um erro de conexão com o banco de dados");
}

FormGestaoFornecedores::~FormGestaoFornecedores()
{
    delete ui;
}

//Fornecedores - GESTÃO
void FormGestaoFornecedores::atualizarFornecedores(QString query, QString msgErro){
    ui->tabelaFornecedores->clearContents();
    ui->tabelaFornecedores->setRowCount(0);

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
                ui->tabelaFornecedores->insertRow(linha);
                ui->tabelaFornecedores->setItem(linha, 0, new QTableWidgetItem(consulta.value("id_fornecedor").toString()));
                ui->tabelaFornecedores->setItem(linha, 1, new QTableWidgetItem(consulta.value("nome").toString()));
                ui->tabelaFornecedores->setItem(linha, 2, new QTableWidgetItem(consulta.value("cnpj").toString()));
                ui->tabelaFornecedores->setItem(linha, 3, new QTableWidgetItem(consulta.value("telefone").toString()));
                ui->tabelaFornecedores->setItem(linha, 4, new QTableWidgetItem(consulta.value("email").toString()));
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

void FormGestaoFornecedores::on_editarButton_clicked(){
    if(ui->tabelaFornecedores->currentRow() != -1){
        int linhaAtual = ui->tabelaFornecedores->currentRow();
        this->con.abrir();
        FormEditarFornecedor *FEF = new FormEditarFornecedor(this, ui->tabelaFornecedores->item(linhaAtual, 0)->text());
        FEF->exec();
        this->con.fechar();
        if(FEF->editado){
            atualizarFornecedores("select * from Fornecedores", "Houve um erro de conexão com o banco de dados!");
            bar->showMessage("Fornecedor editado com sucesso!", 3000);
            bar->setStyleSheet("color: green;");
        }
    }
    else {
        bar->showMessage("Nenhum fornecedor selecionado!", 3000);
        bar->setStyleSheet("color: red;");
    }
}

void FormGestaoFornecedores::on_deletarButton_clicked(){
    if(ui->tabelaFornecedores->currentRow() != -1){
        int linhaAtual = ui->tabelaFornecedores->currentRow();
        QMessageBox::StandardButton res = QMessageBox::question(this, "StockBack", "Deseja deletar o fornecedor " + ui->tabelaFornecedores->item(linhaAtual,1)->text() + "?", QMessageBox::Yes|QMessageBox::No);
        if(res == QMessageBox::Yes){
            this->con.abrir();
            QSqlQuery deletar;
            deletar.prepare("delete from Fornecedores where id_fornecedor=?");
            deletar.addBindValue(ui->tabelaFornecedores->item(linhaAtual, 0)->text());
            if(deletar.exec()){
                ui->tabelaFornecedores->removeRow(linhaAtual);
                bar->showMessage("Fornecedor deletado com sucesso!", 3000);
                bar->setStyleSheet("color: green;");
            }
            this->con.fechar();
        }
    }
    else {
        bar->showMessage("Nenhum fornecedor selecionado!", 3000);
        bar->setStyleSheet("color: red;");
    }
}

void FormGestaoFornecedores::on_filtrarButton_clicked(){
    if(ui->idRadioButton->isChecked()){
        if(ui->busca->text() == ""){
            atualizarFornecedores("select * from Fornecedores", "Houve um erro de conexão com o banco de dados!");
        }
        else {
            QRegularExpression regex ("[^0-9]");
            QRegularExpressionMatch tem = regex.match(ui->busca->text());
            if(tem.hasMatch()){
                bar->showMessage("O código deve conter apenas números!", 3000);
                bar->setStyleSheet("color: red;");
            }
            else {
                atualizarFornecedores("select * from Fornecedores where id_fornecedor=" + ui->busca->text(), "Nenhum fornecedor encontrado!");
            }
        }
    }
    else if(ui->nomeRadioButton->isChecked()){
        if(ui->busca->text() == ""){
            atualizarFornecedores("select * from Fornecedores order by nome", "Houve um erro de conexão com o banco de dados!");
        }
        else {
            atualizarFornecedores("select * from Fornecedores where nome like '%" + ui->busca->text() + "%' order by nome", "Nenhum fornecedor encontrado!");
        }
    }
}

void FormGestaoFornecedores::on_idRadioButton_clicked(){
    ui->busca->setPlaceholderText("Código");
}

void FormGestaoFornecedores::on_nomeRadioButton_clicked(){
    ui->busca->setPlaceholderText("Nome");
}

//Fornecedores - ADICIONAR
void FormGestaoFornecedores::on_criarButton_clicked(){
    if(ui->nome->text() != "" && ui->cnpj->text() != "" && ui->telefone->text() != "" && ui->email->text() != ""){
        this->con.abrir();
        QSqlQuery inserir;
        inserir.prepare("insert into Fornecedores (nome,cnpj,telefone,email) values (?,?,?,?)");
        inserir.addBindValue(ui->nome->text());
        inserir.addBindValue(ui->cnpj->text());
        inserir.addBindValue(ui->telefone->text());
        inserir.addBindValue(ui->email->text());
        if(inserir.exec()){
            on_limparButton_clicked();
            atualizarFornecedores("select * from Fornecedores", "Houve um erro de conexão com o banco de dados!");
            bar->showMessage("Fornecedor cadastrado com sucesso!", 3000);
            bar->setStyleSheet("color: green;");
        }
        this->con.fechar();
    }
    else {
        bar->showMessage("Preencha todos os campos!", 3000);
        bar->setStyleSheet("color: red;");
    }
}

void FormGestaoFornecedores::on_limparButton_clicked(){
    ui->nome->clear();
    ui->cnpj->clear();
    ui->telefone->clear();
    ui->email->clear();

    ui->nome->setFocus();
}

//Fornecedores - REGISTROS
void FormGestaoFornecedores::atualizarRegistros(QString query, QString msgErro){
    ui->tabelaRegistros->clearContents();
    ui->tabelaRegistros->setRowCount(0);

    this->con.abrir();
    QSqlQuery consulta;
    consulta.prepare(query);
    if(consulta.exec()){
        if(!consulta.first()){
            bar->showMessage(msgErro, 3000);
            bar->setStyleSheet("color: orange;");
        }
        else {
            int linha = 0;
            do {
                ui->tabelaRegistros->insertRow(linha);
                ui->tabelaRegistros->setItem(linha, 0, new QTableWidgetItem(consulta.value("id_registro").toString()));
                ui->tabelaRegistros->setItem(linha, 1, new QTableWidgetItem(consulta.value("dataRegistro").toDate().toString("dd/MM/yyyy")));
                ui->tabelaRegistros->setItem(linha, 2, new QTableWidgetItem(consulta.value("horaRegistro").toTime().toString("hh:mm")));
                ui->tabelaRegistros->setItem(linha, 3, new QTableWidgetItem(consulta.value("descricao").toString()));
                ui->tabelaRegistros->setItem(linha, 4, new QTableWidgetItem(consulta.value("quantidade").toString()));
                ui->tabelaRegistros->setItem(linha, 5, new QTableWidgetItem(consulta.value("nome").toString()));
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

void FormGestaoFornecedores::on_registrarAqButton_clicked(){
    this->con.abrir();
    FormNovaAquisicao FNA;
    FNA.exec();
    this->con.fechar();
    if(FNA.criado){
        atualizarRegistros("select Registros.*, Produtos.descricao, Fornecedores.nome from Registros "
                           "inner join Produtos on Produtos.id_produto = Registros.id_produto "
                           "inner join Fornecedores on Fornecedores.id_fornecedor = Registros.id_fornecedor",
                           "Houve um erro de conexão com o banco de dados");
        bar->showMessage("Aquisição registrada!", 3000);
        bar->setStyleSheet("color: green;");
    }
}

void FormGestaoFornecedores::on_deletarAqButton_clicked(){
    if(ui->tabelaRegistros->currentRow() != -1){
        int linhaAtual = ui->tabelaRegistros->currentRow();
        QMessageBox::StandardButton res = QMessageBox::question(this, "StockBack", "Deseja deletar o registro " + ui->tabelaRegistros->item(linhaAtual, 0)->text() + "? Os produtos do registro não serão\ndeletados do estoque, você terá que deletá-los manualmente.", QMessageBox::Yes|QMessageBox::No);
        if(res == QMessageBox::Yes){
            this->con.abrir();
            QSqlQuery deletar;
            deletar.prepare("delete from Registros where id_registro=?");
            deletar.addBindValue(ui->tabelaRegistros->item(linhaAtual, 0)->text());
            if(deletar.exec()){
                ui->tabelaRegistros->removeRow(linhaAtual);
                bar->showMessage("Registro deletado!", 3000);
                bar->setStyleSheet("color: green;");
            }
            this->con.fechar();
        }
    }
    else {
        bar->showMessage("Nenhum registro selecionado!", 3000);
        bar->setStyleSheet("color: red;");
    }
}

void FormGestaoFornecedores::on_filtrarAqButton_clicked(){
    atualizarRegistros("select Registros.*, Produtos.descricao, Fornecedores.nome from Registros "
                           "inner join Produtos on Produtos.id_produto = Registros.id_produto "
                           "inner join Fornecedores on Fornecedores.id_fornecedor = Registros.id_fornecedor "
                           "where dataRegistro between '" + ui->dataInicio->date().toString("yyyy-MM-dd") + "' and '" + ui->dataFim->date().toString("yyyy-MM-dd") + "'",
                           "Nenhum registro encontrado!");
}

void FormGestaoFornecedores::on_restaurarButton_clicked(){
    atualizarRegistros("select Registros.*, Produtos.descricao, Fornecedores.nome from Registros "
                           "inner join Produtos on Produtos.id_produto = Registros.id_produto "
                           "inner join Fornecedores on Fornecedores.id_fornecedor = Registros.id_fornecedor",
                           "Houve um erro de conexão com o banco de dados");
}

