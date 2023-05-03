#include "formgestaofunc.h"
#include "ui_formgestaofunc.h"

#include "formeditarfunc.h"

#include <QMessageBox>
#include <QRegularExpression>

FormGestaoFunc::FormGestaoFunc(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormGestaoFunc)
{
    ui->setupUi(this);
    this->setWindowTitle("StockBack - Gestão de funcionários");
    ui->tabFuncionarios->setCurrentIndex(0);

    //Status bar
    bar = new QStatusBar(this);
    ui->statusbarLayout->addWidget(bar);

    //Settando a tabela de funcionários
    ui->tabelaFuncionarios->setColumnCount(6);
    ui->tabelaFuncionarios->setHorizontalHeaderLabels(QStringList {"ID Func.", "Nome", "Telefone", "E-mail", "Username", "Acesso"});
    ui->tabelaFuncionarios->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tabelaFuncionarios->verticalHeader()->setVisible(false);
    ui->tabelaFuncionarios->setAlternatingRowColors(true);
    ui->tabelaFuncionarios->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tabelaFuncionarios->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tabelaFuncionarios->setStyleSheet("QTableView {selection-background-color: #66696c}");

    atualizarFunc("select * from Funcionarios", "Houve um erro de conexão com o banco de dados!");
}

FormGestaoFunc::~FormGestaoFunc()
{
    delete ui;
}

//Funcionários - GESTÃO
void FormGestaoFunc::atualizarFunc(QString query, QString msgErro){
    ui->tabelaFuncionarios->clearContents();
    ui->tabelaFuncionarios->setRowCount(0);

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
                ui->tabelaFuncionarios->insertRow(linha);
                ui->tabelaFuncionarios->setItem(linha, 0, new QTableWidgetItem(consulta.value("id_funcionario").toString()));
                ui->tabelaFuncionarios->setItem(linha, 1, new QTableWidgetItem(consulta.value("nome").toString()));
                ui->tabelaFuncionarios->setItem(linha, 2, new QTableWidgetItem(consulta.value("telefone").toString()));
                ui->tabelaFuncionarios->setItem(linha, 3, new QTableWidgetItem(consulta.value("email").toString()));
                ui->tabelaFuncionarios->setItem(linha, 4, new QTableWidgetItem(consulta.value("username").toString()));
                QString acesso = "Padrão";
                if(consulta.value("acesso").toString() == "A"){
                    acesso = "Admin";
                }
                ui->tabelaFuncionarios->setItem(linha, 5, new QTableWidgetItem(acesso));
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

void FormGestaoFunc::on_editarButton_clicked(){
    if(ui->tabelaFuncionarios->currentRow() != -1){
        int linha = ui->tabelaFuncionarios->currentRow();
        this->con.abrir();
        FormEditarFunc *FEF = new FormEditarFunc(this, ui->tabelaFuncionarios->item(linha, 0)->text());
        FEF->exec();
        this->con.fechar();
        if(FEF->editado){
            atualizarFunc("select * from Funcionarios", "Houve um erro de conexão com o banco de dados!");
            bar->showMessage("Funcionário editado com sucesso!", 3000);
            bar->setStyleSheet("color: green;");
        }
    }
    else {
        bar->showMessage("Nenhum funcionário selecionado!", 3000);
        bar->setStyleSheet("color: red;");
    }
}

void FormGestaoFunc::on_deletarButton_clicked(){
    if(ui->tabelaFuncionarios->currentRow() != -1){
        int linha = ui->tabelaFuncionarios->currentRow();
        QMessageBox::StandardButton res = QMessageBox::question(this, "StockBack", "Deseja deletar o funcionário(a) " + ui->tabelaFuncionarios->item(linha, 1)->text() + "?", QMessageBox::Yes|QMessageBox::No);
        if(res == QMessageBox::Yes){
            this->con.abrir();
            QSqlQuery deletar;
            deletar.prepare("delete from Funcionarios where id_funcionario=?");
            deletar.addBindValue(ui->tabelaFuncionarios->item(linha, 0)->text());
            if(deletar.exec()){
                ui->tabelaFuncionarios->removeRow(linha);
                bar->showMessage("Funcionário deletado com sucesso!", 3000);
                bar->setStyleSheet("color: green;");
            }
            else {
                bar->showMessage("Houve um erro ao deletar o funcionário!", 3000);
                bar->setStyleSheet("color: red;");
            }
            this->con.fechar();
        }
    }
    else {
        bar->showMessage("Nenhum funcionário selecionado!", 3000);
        bar->setStyleSheet("color: red;");
    }
}

void FormGestaoFunc::on_filtrarButton_clicked(){
    if(ui->idRadioButton->isChecked()){
        if(ui->busca->text() == ""){
            atualizarFunc("select * from Funcionarios", "Houve um erro de conexão com o banco de dados!");
        }
        else {
            QRegularExpression regex ("[^0-9]");
            QRegularExpressionMatch tem = regex.match(ui->busca->text());
            if(tem.hasMatch()){
                bar->showMessage("O ID do funcionário deve conter apenas números!", 3000);
                bar->setStyleSheet("color: red;");
            }
            else {
                atualizarFunc("select * from Funcionarios where id_funcionario=" + ui->busca->text(), "Nenhum funcionário encontrado!");
                ui->busca->clear();
            }
        }
    }
    else if(ui->nomeRadioButton->isChecked()){
        if(ui->busca->text() == ""){
            atualizarFunc("select * from Funcionarios order by nome", "Houve um erro de conexão com o banco de dados!");
        }
        else {
            atualizarFunc("select * from Funcionarios where nome like '%" + ui->busca->text() + "%' order by nome", "Nenhum funcionário encontrado!");
            ui->busca->clear();
        }
    }
}

void FormGestaoFunc::on_idRadioButton_clicked(){
    ui->busca->setPlaceholderText("ID do funcionário");
}

void FormGestaoFunc::on_nomeRadioButton_clicked(){
    ui->busca->setPlaceholderText("Nome");
}


//Funcionários - ADICIONAR
void FormGestaoFunc::on_criarButton_clicked(){
    if(ui->username->text() == "" || ui->senha->text() == "" || ui->nome->text() == "" || ui->telefone->text() == "" || ui->email->text() == ""){
        bar->showMessage("Preencha todos os campos!", 3000);
        bar->setStyleSheet("color: red;");
    }
    else {
        this->con.abrir();
        QSqlQuery query;
        query.prepare("select username from Funcionarios where username=?");
        query.addBindValue(ui->username->text());
        if(query.exec()){
            if(query.first()){
                bar->showMessage("O username escolhido já pertence à outro usuário!", 4500);
                bar->setStyleSheet("color: red;");
            }
            else {
                query.prepare("insert into Funcionarios (username,senha,nome,telefone,email,acesso) values (?,?,?,?,?,?)");
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
                if(query.exec()){
                    on_limparButton_clicked();
                    atualizarFunc("select * from Funcionarios", "Houve um erro de conexão com o banco de dados!");
                    bar->showMessage("Usuário criado!", 3000);
                    bar->setStyleSheet("color: green;");
                }
                else {
                    bar->showMessage("Houve um erro ao criar o novo usuário!", 3000);
                    bar->setStyleSheet("color: red;");
                }
            }
        }
        this->con.fechar();
    }
}

void FormGestaoFunc::on_limparButton_clicked(){
    ui->username->clear();
    ui->senha->clear();
    ui->nome->clear();
    ui->telefone->clear();
    ui->email->clear();
    ui->acesso->setCurrentIndex(0);

    ui->username->setFocus();
}

