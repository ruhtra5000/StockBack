#ifndef FORMGESTAOESTOQUE_H
#define FORMGESTAOESTOQUE_H

#include "Conexao.h"

#include <QDialog>
#include <QtSql>
#include <QStatusBar>

namespace Ui {
class FormGestaoEstoque;
}

class FormGestaoEstoque : public QDialog
{
    Q_OBJECT

public:
    explicit FormGestaoEstoque(QWidget *parent = nullptr);
    ~FormGestaoEstoque();

private slots:
    //Estoque - GESTÃO
    void atualizarProdutos(QString query, QString msgErro);

    void on_editarButton_clicked();

    void on_deletarButton_clicked();

    void on_filtrarButton_clicked();

    void on_codigoRadioButton_clicked();

    void on_descRadioButton_clicked();

    //Estoque - ADICIONAR PRODUTO
    void on_criarButton_clicked();

    void on_limparButton_clicked();

    void setCodigoProduto();

private:
    Ui::FormGestaoEstoque *ui;
    Conexao con;
    QStatusBar *bar;
};

#endif // FORMGESTAOESTOQUE_H
