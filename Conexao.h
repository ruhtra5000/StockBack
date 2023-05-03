#ifndef CONEXAO_H
#define CONEXAO_H

#include <QtSql>
#include <QApplication>

class Conexao {
private:
    QSqlDatabase BD;
    QString local;
    QString banco;
public:
    Conexao (){
        this->BD = QSqlDatabase::addDatabase("QSQLITE");
        this->local = qApp->applicationDirPath();
        this->banco = local + "/bancoDeDados/StockBack.db";
    }

    bool abrir(){
        this->BD.setDatabaseName(this->banco);
        qDebug() << banco;
        if(this->BD.open()){
            return true;
        }
        qDebug() << this->BD.lastError();
        return false;
    }

    void fechar(){
        this->BD.close();
    }

    bool estaAberto(){
        if(this->BD.isOpen()){
            return true;
        }
        return false;
    }

    void reabrirConexao(){
        this->BD = QSqlDatabase::addDatabase("QSQLITE");
    }
};

#endif // CONEXAO_H
