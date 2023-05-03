#include "formnovavenda.h"
#include "ui_formnovavenda.h"

#include "formprincipal.h"
#include "formpesquisarproduto.h"
#include "formeditarprodutovenda.h"

#include <QMessageBox>
#include <QPrinter>
#include <QPainter>
#include <QDesktopServices>

FormNovaVenda::FormNovaVenda(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormNovaVenda)
{
    ui->setupUi(this);
    this->setWindowTitle("StockBack - Nova venda");

    //Status bar
    bar = new QStatusBar(this);
    ui->statusbarLayout->addWidget(bar);

    //Configurando a tabela
    ui->tabelaVenda->setColumnCount(5);
    ui->tabelaVenda->setHorizontalHeaderLabels(QStringList {"Código", "Produto", "Qtde", "Valor Un.", "Total"});
    ui->tabelaVenda->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tabelaVenda->verticalHeader()->setVisible(false);
    ui->tabelaVenda->setAlternatingRowColors(true);
    ui->tabelaVenda->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tabelaVenda->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tabelaVenda->setStyleSheet("QTableView {selection-background-color: #66696c}");
}

FormNovaVenda::~FormNovaVenda()
{
    delete ui;
}

void FormNovaVenda::on_adicionarButton_clicked(){
    if(ui->codProd->value() == 0 || ui->quantidade->value() == 0){
        bar->showMessage("Preencha todos os campos!", 3000);
        bar->setStyleSheet("color: red;");
    }
    else {
        this->con.abrir();
        QSqlQuery query;
        query.prepare("select id_produto,descricao,quantidade,valorVenda from Produtos where id_produto=?");
        query.addBindValue(QString::number(ui->codProd->value()));
        if(query.exec()){
            //Checando se o código existe
            if(!query.first()){
                bar->showMessage("O código de produto informado não existe!", 3000);
                bar->setStyleSheet("color: red;");
            }
            else {
                //Checando se o produto já consta na venda
                if(estaNaVenda(query.value("id_produto").toString())){
                    bar->showMessage("O produto já consta na venda!", 3500);
                    bar->setStyleSheet("color: red;");
                }
                else {
                    //Checando a quantidade em estoque
                    if(ui->quantidade->value() > query.value("quantidade").toInt()){
                        bar->showMessage("Há apenas " + query.value("quantidade").toString() + " unidades deste produto em estoque!", 3000);
                        bar->setStyleSheet("color: red;");
                    }
                    else {
                        ui->tabelaVenda->insertRow(this->linha);
                        ui->tabelaVenda->setItem(linha, 0, new QTableWidgetItem(query.value("id_produto").toString()));
                        ui->tabelaVenda->setItem(linha, 1, new QTableWidgetItem(query.value("descricao").toString()));
                        ui->tabelaVenda->setItem(linha, 2, new QTableWidgetItem(QString::number(ui->quantidade->value())));
                        ui->tabelaVenda->setItem(linha, 3, new QTableWidgetItem("R$ " + query.value("valorVenda").toString()));
                        ui->tabelaVenda->setItem(linha, 4, new QTableWidgetItem("R$ " + QString::number(ui->quantidade->value() * query.value("valorVenda").toDouble())));
                        this->linha++;

                        calcularValorTotal();
                        ui->codProd->setValue(0);
                        ui->quantidade->setValue(1);
                        ui->codProd->setFocus();
                    }
                }
            }
        }
        this->con.fechar();
    }
}

void FormNovaVenda::on_pesquisarButton_clicked(){
    this->con.abrir();
    FormPesquisarProduto FPP;
    FPP.exec();
    this->con.fechar();
    if(FPP.selec){
        ui->codProd->setValue(FPP.codProd);
    }
}

void FormNovaVenda::on_editarButton_clicked(){
    if(ui->tabelaVenda->currentRow() == -1){
        bar->showMessage("Nenhum produto selecionado!", 3000);
        bar->setStyleSheet("color: red;");
    }
    else {
        int linhaSelec = ui->tabelaVenda->currentRow();
        QString valor = ui->tabelaVenda->item(linhaSelec, 3)->text();
        valor.erase(valor.begin(), valor.begin()+3);
        this->con.abrir();
        FormEditarProdutoVenda *FEPV = new FormEditarProdutoVenda(this,
        ui->tabelaVenda->item(linhaSelec, 0)->text(), ui->tabelaVenda->item(linhaSelec, 1)->text(),
        ui->tabelaVenda->item(linhaSelec, 2)->text().toInt(), valor.toDouble());
        FEPV->exec();
        this->con.fechar();
        if(FEPV->editado){
            ui->tabelaVenda->setItem(linhaSelec, 2, new QTableWidgetItem(QString::number(FEPV->quant)));
            ui->tabelaVenda->setItem(linhaSelec, 3, new QTableWidgetItem("R$ " + QString::number(FEPV->valor)));
            ui->tabelaVenda->setItem(linhaSelec, 4, new QTableWidgetItem("R$ " + QString::number(FEPV->quant*FEPV->valor)));
            calcularValorTotal();
            bar->showMessage("Produto editado com sucesso!", 3000);
            bar->setStyleSheet("color: green;");
        }
    }
}

void FormNovaVenda::on_deletarButton_clicked(){
    if(ui->tabelaVenda->currentRow() == -1){
        bar->showMessage("Nenhum produto selecionado!", 3000);
        bar->setStyleSheet("color: red;");
    }
    else {
        int linhaSelec = ui->tabelaVenda->currentRow();
        QMessageBox::StandardButton res = QMessageBox::question(this, "StockBack", "Deseja deletar o produto " + ui->tabelaVenda->item(linhaSelec, 1)->text() + "?", QMessageBox::Yes|QMessageBox::No);
        if(res == QMessageBox::Yes){
            ui->tabelaVenda->removeRow(linhaSelec);
            calcularValorTotal();
            this->linha--;

            bar->showMessage("Produto deletado!", 3000);
            bar->setStyleSheet("color: green;");
        }
    }
}

void FormNovaVenda::on_finalizarButton_clicked(){
    if(ui->tabelaVenda->rowCount() > 0){
        this->con.abrir();
        QString valorTotal = ui->valorTotal->text();
        valorTotal.erase(valorTotal.begin(), valorTotal.begin()+16);

        //Criando a venda
        QSqlQuery query;
        query.prepare("insert into Vendas (dataVenda, horaVenda, valorTotal, id_funcionario, id_tipo_pagamento, fechado) values (?, ?, ?, ?, ?, ?)");
        query.addBindValue(QDate::currentDate().toString("yyyy-MM-dd"));
        query.addBindValue(QTime::currentTime().toString("hh:mm:ss"));
        query.addBindValue(valorTotal);
        query.addBindValue(QString::number(FormPrincipal::idFunc));
        query.addBindValue("1");
        query.addBindValue(QString::number(this->fechar));
        if(query.exec()){
            //Pegando o id da venda
            query.prepare("select id_venda from Vendas order by id_venda desc limit 1");
            query.exec();
            if(query.first()){
                QString idVenda = query.value("id_venda").toString();
                QString tmp1, tmp2;
                for(int n = 0; n < ui->tabelaVenda->rowCount(); n++){
                    //Inserindo os produtos no produtosVendas
                    query.prepare("insert into produtosVendas (id_venda,id_produto,descricao,quantidade,valorUnidade,valorTotal) values (?,?,?,?,?,?)");
                    query.addBindValue(idVenda);
                    query.addBindValue(ui->tabelaVenda->item(n, 0)->text());
                    query.addBindValue(ui->tabelaVenda->item(n, 1)->text());
                    query.addBindValue(ui->tabelaVenda->item(n, 2)->text());
                    tmp1 = ui->tabelaVenda->item(n, 3)->text();//valor da unidade
                    tmp1.erase(tmp1.begin(), tmp1.begin()+3);
                    query.addBindValue(tmp1);
                    tmp2 = ui->tabelaVenda->item(n, 4)->text();//valor total da linha
                    tmp2.erase(tmp2.begin(), tmp2.begin()+3);
                    query.addBindValue(tmp2);
                    if(query.exec()){
                        query.prepare("update Produtos set quantidade = quantidade - ? where id_produto=?");
                        query.addBindValue(ui->tabelaVenda->item(n, 2)->text());
                        query.addBindValue(ui->tabelaVenda->item(n, 0)->text());
                        query.exec();
                    }
                }
                QMessageBox::information(this, "StockBack",
                "Venda finalizada!\nID da venda: " + idVenda +
                "\nValor total: R$ " + valorTotal);
                this->vendaAberta = true;
                //Gerar a nota se a venda for fechada
                if(this->fechar == 1){
                    gerarNota(idVenda);
                }
                this->close();
            }
        }
        this->con.fechar();
    }
    else {
        bar->showMessage("Sem produtos na venda!", 3000);
        bar->setStyleSheet("color: red;");
        this->fechar = 0;
    }
}

void FormNovaVenda::on_finalizarFecharButton_clicked(){
    this->fechar = 1;
    on_finalizarButton_clicked();
    this->vendaAberta = false;
}

void FormNovaVenda::gerarNota(QString id){
    QString nome = QDir::currentPath() + "/notas/"+id+"venda.pdf";
    QPrinter printer;
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(nome);

    QPainter painter;
    if(!painter.begin(&printer)){
        bar->showMessage("Erro ao abrir PDF!", 3000);
        bar->setStyleSheet("color: red;");
        return;
    }

    int linha = 300;
    int salto = 25;

    //Logo do programa
    painter.drawPixmap(25, 25, QPixmap(":/imgs/imagens/logoMedio.png"));

    //Dados da venda
    this->con.abrir();
    QSqlQuery consulta;
    consulta.prepare("select Vendas.*, Funcionarios.nome from Vendas "
                     "inner join Funcionarios on Funcionarios.id_funcionario = Vendas.id_funcionario "
                     "where Vendas.id_venda=?");
    consulta.addBindValue(id);
    if(consulta.exec()){
        consulta.first();
        painter.drawText(340, 35, consulta.value("id_venda").toString());
        painter.drawText(340, 60, "Data: " + consulta.value("dataVenda").toDate().toString("dd/MM/yyyy"));
        painter.drawText(340, 85, "Hora: " + consulta.value("horaVenda").toString());
        painter.drawText(340, 110, "Responsável: " + consulta.value("nome").toString());
        painter.drawText(340, 135, "Valor total: R$ " + QString::number(consulta.value("valorTotal").toDouble(), 'f', 2));
        painter.drawLine(QPoint(25, 230), QPoint(750, 230));

        //Produtos da venda
        consulta.prepare("select descricao,quantidade,valorUnidade,valorTotal from produtosVendas where id_venda=?");
        consulta.addBindValue(id);
        if(consulta.exec()){
            //Cabeçalho
            painter.drawText(30, 255, "Produto");
            painter.drawText(230, 255, "Quantidade");
            painter.drawText(430, 255, "Valor unitário");
            painter.drawText(630, 255, "Valor total");
            painter.drawLine(QPoint(25, 270), QPoint(750, 270));
            while(consulta.next()){
                painter.drawText(30, linha, consulta.value("descricao").toString());
                painter.drawText(230, linha, consulta.value("quantidade").toString());
                painter.drawText(430, linha, "R$ " + QString::number(consulta.value("valorUnidade").toDouble(), 'f', 2));
                painter.drawText(630, linha, "R$ " + QString::number(consulta.value("valorTotal").toDouble(), 'f', 2));
                linha += salto;
            }

            linha -= 10;
            painter.drawLine(QPoint(200, 255), QPoint(200, linha));
            painter.drawLine(QPoint(400, 255), QPoint(400, linha));
            painter.drawLine(QPoint(600, 255), QPoint(600, linha));
        }
    }
    this->con.fechar();

    //Finalizando o documento
    painter.end();
    QDesktopServices::openUrl(QUrl("file:///"+nome));
}

bool FormNovaVenda::estaNaVenda(QString codigo){
    if(ui->tabelaVenda->rowCount() < 1){
        return false;
    }
    for(int i = 0; i < ui->tabelaVenda->rowCount(); i++){
        if(ui->tabelaVenda->item(i, 0)->text() == codigo){
            ui->tabelaVenda->selectRow(i);
            return true;
        }
    }
    return false;
}

void FormNovaVenda::calcularValorTotal(){
    double valor = 0; QString v;
    if(ui->tabelaVenda->rowCount() > 0){
        for(int i = 0; i < ui->tabelaVenda->rowCount(); i++){
            v = ui->tabelaVenda->item(i, 4)->text();
            v.erase(v.begin(), v.begin() + 3);
            valor += v.toDouble();
        }
    }
    ui->valorTotal->setText("Valor total: R$ " + QString::number(valor));
}
