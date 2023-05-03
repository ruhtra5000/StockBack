#include "formprincipal.h"
#include "./ui_formprincipal.h"

#include "formlogin.h"
#include "formnovavenda.h"
#include "formgestaoestoque.h"
#include "formgestaofunc.h"
#include "formgestaovendas.h"
#include "formgestaofornecedores.h"

#include <QMessageBox>
#include <QHeaderView>
#include <QPrinter>
#include <QPainter>
#include <QDesktopServices>

int FormPrincipal::idFunc;
QString FormPrincipal::nomeFunc;
QString FormPrincipal::acessoFunc;

FormPrincipal::FormPrincipal(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::FormPrincipal)
{
    ui->setupUi(this);
    this->setWindowTitle("StockBack");

    //Settando os dados padrão de usuário
    FormPrincipal::idFunc = 0;
    FormPrincipal::nomeFunc = "Sem usuário logado";

    //Toolbar
    QWidget *espaco = new QWidget();
    espaco->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    ui->toolBar->insertWidget(ui->actionLogin, espaco);

    nomeUsuario = new QLabel();
    nomeUsuario->setText(FormPrincipal::nomeFunc);
    nomeUsuario->setStyleSheet("margin-right: 3px;");
    nomeUsuario->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    ui->toolBar->insertWidget(ui->actionLogin, nomeUsuario);

    //Botões (deletar e fechar venda)
    connect(this, &FormPrincipal::fecharSignal, this, &FormPrincipal::fecharVenda);
    connect(this, &FormPrincipal::deletarSignal, this, &FormPrincipal::deletarVenda);
}

FormPrincipal::~FormPrincipal()
{
    delete ui;
}

//Tela Principal
void FormPrincipal::limparVendas(){
    if(ui->layoutVendas->count() > 0){
        //tem vendas abertas
        if(this->vendaAberta){
            QLayoutItem *item, *item2; QLayout *l;
            while((item = ui->layoutVendas->itemAt(0)) != 0){
                //item Qframe
                l = item->widget()->layout();
                while((item2 = l->itemAt(0)) != 0){
                    delete item2->widget();
                }
                delete l;
                delete item->widget();
            }
        }
        //nao tem vendas abertas
        else {
            QLayoutItem *item = ui->layoutVendas->itemAt(0);
            delete item->widget();
        }
    }
}

void FormPrincipal::atualizarVendasAbertas(bool limpar){
    if(limpar){
        limparVendas();
    }

    if(FormPrincipal::idFunc != 0){
        QString txtQuery = "select Vendas.*, Funcionarios.nome from Vendas inner join Funcionarios on Vendas.id_funcionario = Funcionarios.id_funcionario where fechado = 0";
        if(FormPrincipal::acessoFunc == "B"){
            txtQuery = txtQuery + " and Vendas.id_funcionario = " + QString::number(FormPrincipal::idFunc);
        }
        this->con.abrir();
        QSqlQuery consulta;
        consulta.prepare(txtQuery);
        if(consulta.exec()){
            if(!consulta.first()){
                QLabel *textoErro = new QLabel();
                textoErro->setText("<div style=\"text-align: center;\"><h2>Nenhuma venda aberta</h2></div>");
                ui->layoutVendas->addWidget(textoErro);
                this->vendaAberta = false;
            }
            else {
                //Criando o cabeçalho
                QWidget *cabec = new QWidget();
                cabec->setObjectName("cabecalho");
                cabec->setFixedHeight(40);
                cabec->setStyleSheet("#cabecalho { border-bottom: 1px solid black; }");

                QHBoxLayout *layoutCabec = new QHBoxLayout();
                QLabel *cabec1 = new QLabel();
                cabec1->setText("<div style=\"text-align: center;\">ID venda</div>");
                cabec1->setMaximumWidth(80);
                QLabel *cabec2 = new QLabel();
                cabec2->setText("<div style=\"text-align: center;\">Data e hora</div>");
                QLabel *cabec3 = new QLabel();
                cabec3->setText("<div style=\"text-align: center;\">Responsável</div>");
                QLabel *cabec4 = new QLabel();
                cabec4->setText("<div style=\"text-align: center;\">Valor</div>");
                QLabel *cabec5 = new QLabel();
                cabec5->setMaximumWidth(90);
                cabec5->setText("<div style=\"text-align: center;\">Funções</div>");

                layoutCabec->addWidget(cabec1);
                layoutCabec->addWidget(cabec2);
                layoutCabec->addWidget(cabec3);
                layoutCabec->addWidget(cabec4);
                layoutCabec->addWidget(cabec5);
                cabec->setLayout(layoutCabec);
                ui->layoutVendas->addWidget(cabec, 0, Qt::AlignTop);

                QString idVenda;
                do {
                    //Criando os elementos de cada instancia venda
                    QFrame *frame = new QFrame();
                    frame->setFixedHeight(60);
                    frame->setStyleSheet("QFrame {background-color: rgba(0, 0, 0, 0);} QFrame:hover {background-color: #dddddd;}");

                    QHBoxLayout *layout = new QHBoxLayout();

                    //Dados das vendas
                    QLabel *dados1 = new QLabel();
                    dados1->setText("<div style=\"text-align: center;\">" + consulta.value("id_venda").toString() + "</div>");
                    dados1->setMaximumWidth(80);
                    dados1->setStyleSheet("QLabel { border-right: 1px solid black; padding-right: 1px;}");

                    QLabel *dados2 = new QLabel();
                    dados2->setText("<div style=\"text-align: center;\">" + consulta.value("dataVenda").toDate().toString("dd/MM/yyyy") +
                                    " - " + consulta.value("horaVenda").toString() + "</div>");
                    dados2->setStyleSheet("QLabel { border-right: 1px solid black; padding-right: 1px;}");

                    QLabel *dados3 = new QLabel();
                    dados3->setText("<div style=\"text-align: center;\">" + consulta.value("nome").toString() + "</div>");
                    dados3->setStyleSheet("QLabel { border-right: 1px solid black; padding-right: 1px;}");

                    QLabel *dados4 = new QLabel();
                    dados4->setText("<div style=\"text-align: center;\">R$ " + QString::number(consulta.value("valorTotal").toDouble(), 'f', 2) + "</div>");
                    dados4->setStyleSheet("QLabel { border-right: 1px solid black; padding-right: 1px;}");

                    idVenda = consulta.value("id_venda").toString();

                    QPushButton *fecharButton = new QPushButton();
                    fecharButton->setMaximumSize(40, 40);
                    fecharButton->setIcon(this->fecharIcon);
                    fecharButton->setIconSize(QSize(20, 20));
                    fecharButton->setToolTip("Fechar venda");
                    fecharButton->setStyleSheet("QPushButton {background-color: #5CD63C; border-radius: 5px;} :hover {background-color: #53c136;}");
                    connect(fecharButton, &QPushButton::clicked, this, [idVenda, this](){
                        emit fecharSignal(idVenda);
                    });

                    QPushButton *delButton = new QPushButton();
                    delButton->setMaximumSize(40, 40);
                    delButton->setIcon(this->delIcon);
                    delButton->setIconSize(QSize(18, 18));
                    delButton->setToolTip("Deletar venda");
                    delButton->setStyleSheet("QPushButton {background-color: #ec0d0d; border-radius: 5px;} :hover {background-color: #d40c0c}");
                    connect(delButton, &QPushButton::clicked, this, [idVenda, this]{
                        emit deletarSignal(idVenda);
                    });

                    //Adicionando os elementos em seus conteineres
                    layout->addWidget(dados1);
                    layout->addWidget(dados2);
                    layout->addWidget(dados3);
                    layout->addWidget(dados4);
                    layout->addWidget(fecharButton);
                    layout->addWidget(delButton);
                    frame->setLayout(layout);
                    ui->layoutVendas->addWidget(frame, 0, Qt::AlignTop);
                }
                while(consulta.next());
                this->vendaAberta = true;
            }
        }
        this->con.fechar();
    }
}

void FormPrincipal::on_novaVendaButton_clicked(){
    if(FormPrincipal::idFunc != 0){
        FormNovaVenda FNV;
        FNV.exec();
        this->con.reabrirConexao();
        if(FNV.vendaAberta){
            atualizarVendasAbertas(true);
        }
    }
    else {
        QMessageBox::warning(this, "StockBack", "Sem usuário logado!");
    }
}

void FormPrincipal::fecharVenda(QString id){
    QMessageBox::StandardButton res = QMessageBox::question(this, "StockBack", "Deseja fechar a venda " + id + "?", QMessageBox::Yes|QMessageBox::No);
    if(res == QMessageBox::Yes){
        this->con.abrir();
        QSqlQuery query;
        query.prepare("update Vendas set fechado = 1 where id_venda=?");
        query.addBindValue(id);
        if(query.exec()){
            atualizarVendasAbertas(true);
            ui->statusbar->showMessage("Venda fechada com sucesso!", 3000);
            ui->statusbar->setStyleSheet("color: green;");
            gerarNota(id);
        }
        this->con.fechar();
    }
}

void FormPrincipal::deletarVenda(QString id){
    QMessageBox::StandardButton res = QMessageBox::question(this, "StockBack", "Deseja DELETAR a venda " + id + "?\nOs dados desta venda serão perdidos!", QMessageBox::Yes|QMessageBox::No);
    if(res == QMessageBox::Yes){
        this->con.abrir();
        QSqlQuery query;
        //Pegando os produtos da venda
        query.prepare("select id_produto,quantidade from produtosVendas where id_venda=?");
        query.addBindValue(id);
        if(query.exec()){
            //Voltando os produtos para o estoque
            QSqlQuery atualizar;
            while(query.next()){
                atualizar.prepare("update Produtos set quantidade = quantidade + ? where id_produto=?");
                atualizar.addBindValue(query.value("quantidade").toString());
                atualizar.addBindValue(query.value("id_produto").toString());
                atualizar.exec();
            }

            //Deletando os produtos da venda
            query.prepare("delete from produtosVendas where id_venda=?");
            query.addBindValue(id);
            if(query.exec()){
                //Deletando a venda
                query.prepare("delete from Vendas where id_venda=?");
                query.addBindValue(id);
                if(query.exec()){
                    atualizarVendasAbertas(true);
                    ui->statusbar->showMessage("Venda deletada com sucesso!", 3000);
                    ui->statusbar->setStyleSheet("color: green;");
                }
            }
        }
        this->con.fechar();
    }
}

void FormPrincipal::gerarNota(QString id){
    QString nome = QDir::currentPath() + "/notas/"+id+"venda.pdf";
    QPrinter printer;
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(nome);

    QPainter painter;
    if(!painter.begin(&printer)){
        ui->statusbar->showMessage("Erro ao abrir PDF!", 3000);
        ui->statusbar->setStyleSheet("color: red;");
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

//Menu superior - GESTÃO
void FormPrincipal::on_actionEstoque_triggered(){
    if(adminLogado()){
        FormGestaoEstoque FGE;
        FGE.exec();
        this->con.reabrirConexao();
    }
}

void FormPrincipal::on_actionFuncion_rios_triggered(){
    if(adminLogado()){
        FormGestaoFunc FGF;
        FGF.exec();
        this->con.reabrirConexao();
    }
}

void FormPrincipal::on_actionVendas_triggered(){
    if(adminLogado()){
        FormGestaoVendas FGV;
        FGV.exec();
        this->con.reabrirConexao();
    }
}

void FormPrincipal::on_actionFornecedores_triggered(){
    if(adminLogado()){
        FormGestaoFornecedores FGF;
        FGF.exec();
        this->con.reabrirConexao();
    }
}

bool FormPrincipal::adminLogado(){
    if(FormPrincipal::idFunc == 0){
        QMessageBox::warning(this, "StockBack", "Sem usuário logado!");
        return false;
    }
    else if(FormPrincipal::acessoFunc != "A") {
        QMessageBox::warning(this, "StockBack", "Permissão de administrador necessária!");
        return false;
    }
    return true;
}

//Menu superior - SISTEMA
void FormPrincipal::on_actionSair_triggered(){
    if(FormPrincipal::idFunc != 0){
        QMessageBox::StandardButton res = QMessageBox::question(this, "StockBack", "Deseja fechar o programa? Sua conta ainda está logada!", QMessageBox::Yes|QMessageBox::No);
        if(res == QMessageBox::Yes){
            this->close();
        }
    }
    else {
        this->close();
    }
}

void FormPrincipal::on_actionStockBack_triggered(){
    QMessageBox::about(this, "StockBack",
    "O StockBack é um programa de adminstração desenvolvido por\n"
    "Arthur de Sá Tenório com o objetivo de treinar o uso da IDE Qt Creator!\n\n"
    "Sites usados durante o desenvolvimento:\n"
    "Canal CFB Cursos: https://www.youtube.com/@cfbcursos\n"
    "Documentação do Qt: https://doc.qt.io/\n"
    "Ícones: https://icons8.com.br/\n"
    "Cores: https://imagecolorpicker.com/pt-pt\n"
    "Pixel art: https://www.pixilart.com/");
}

void FormPrincipal::on_actionSobre_o_Qt_Creator_triggered(){
    QMessageBox::aboutQt(this, "StockBack - Sobre o Qt Creator");
}

//Toolbar
void FormPrincipal::on_actionLogin_triggered(){
    //Logar
    if(FormPrincipal::idFunc == 0){
        this->con.abrir();
        FormLogin *FL = new FormLogin(this);
        FL->exec();
        this->con.fechar();

        if(FormPrincipal::idFunc != 0){
            nomeUsuario->setText(FormPrincipal::nomeFunc);
            ui->actionLogin->setIcon(QIcon{":/imgs/imagens/logout.png"});
            ui->actionLogin->setToolTip("Log Out");
            atualizarVendasAbertas(true);
        }
    }
    //Deslogar
    else {
        FormPrincipal::idFunc = 0;
        FormPrincipal::nomeFunc = "Sem usuário logado";
        FormPrincipal::acessoFunc = "B";
        nomeUsuario->setText(FormPrincipal::nomeFunc);
        ui->actionLogin->setIcon(QIcon{":/imgs/imagens/login.png"});
        ui->actionLogin->setToolTip("Login");
        limparVendas();
    }

}

