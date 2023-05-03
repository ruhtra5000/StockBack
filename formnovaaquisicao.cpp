#include "formnovaaquisicao.h"
#include "ui_formnovaaquisicao.h"

#include "formpesquisaraquisicao.h"

#include <QMessageBox>

FormNovaAquisicao::FormNovaAquisicao(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormNovaAquisicao)
{
    ui->setupUi(this);
    this->setWindowTitle("StockBack - Registrar aquisição");
}

FormNovaAquisicao::~FormNovaAquisicao()
{
    delete ui;
}

void FormNovaAquisicao::on_registrarButton_clicked(){
    if(ui->codForn->value() != 0 && ui->codProd->value() != 0 && ui->quantidade->value() != 0){
        QSqlQuery query;
        //Checando se o fornecedor existe
        query.prepare("select id_fornecedor from Fornecedores where id_fornecedor=?");
        query.addBindValue(QString::number(ui->codForn->value()));
        query.exec();
        if(!query.first()){
            QMessageBox::warning(this, "StockBack", "O fornecedor informado não existe!");
            return;
        }

        //Checando se o produto existe
        query.prepare("select id_produto from Produtos where id_produto=?");
        query.addBindValue(QString::number(ui->codProd->value()));
        query.exec();
        if(!query.first()){
            QMessageBox::warning(this, "StockBack", "O produto informado não existe!");
            return;
        }

        //Criando o registro
        query.prepare("insert into Registros (dataRegistro, horaRegistro, id_produto, quantidade, id_fornecedor) values (?, ?, ?, ?, ?)");
        query.addBindValue(ui->dataAq->date().toString("yyyy-MM-dd"));
        query.addBindValue(ui->horaAq->time().toString());
        query.addBindValue(QString::number(ui->codProd->value()));
        query.addBindValue(QString::number(ui->quantidade->value()));
        query.addBindValue(QString::number(ui->codForn->value()));
        if(query.exec()){
            //Inserindo os produtos no estoque
            query.prepare("update Produtos set quantidade = quantidade + ? where id_produto=?");
            query.addBindValue(QString::number(ui->quantidade->value()));
            query.addBindValue(QString::number(ui->codProd->value()));
            if(query.exec()){
                this->criado = true;
                this->close();
                return;
            }
        }
    }
    else {
        QMessageBox::warning(this, "StockBack", "Preencha todos os campos");
        return;
    }
}

void FormNovaAquisicao::on_pesquisarFornButton_clicked(){
    FormPesquisarAquisicao *FPE = new FormPesquisarAquisicao(this, 1);
    FPE->exec();
    if(FPE->codSelec != "0"){
        ui->codForn->setValue(FPE->codSelec.toInt());
    }
}

void FormNovaAquisicao::on_pesquisarProdButton_clicked(){
    FormPesquisarAquisicao *FPE = new FormPesquisarAquisicao(this, 2);
    FPE->exec();
    if(FPE->codSelec != "0"){
        ui->codProd->setValue(FPE->codSelec.toInt());
    }
}

void FormNovaAquisicao::on_dataCheckBox_stateChanged(int arg1){
    //foi desmarcado
    if(arg1 == 0){
        ui->dataAq->setDate(QDate(2000,1,1));
        ui->dataAq->setReadOnly(false);
    }
    //foi marcado
    else {
        ui->dataAq->setDate(QDate::currentDate());
        ui->dataAq->setReadOnly(true);
    }
}

void FormNovaAquisicao::on_horaCheckBox_stateChanged(int arg1){
    if(arg1 == 0){
        ui->horaAq->setTime(QTime(0,0));
        ui->horaAq->setReadOnly(false);
    }
    else {
        ui->horaAq->setTime(QTime::currentTime());
        ui->horaAq->setReadOnly(true);
    }
}
