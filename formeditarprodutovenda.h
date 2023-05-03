#ifndef FORMEDITARPRODUTOVENDA_H
#define FORMEDITARPRODUTOVENDA_H

#include <QDialog>
#include <QtSql>

namespace Ui {
class FormEditarProdutoVenda;
}

class FormEditarProdutoVenda : public QDialog
{
    Q_OBJECT

public:
    explicit FormEditarProdutoVenda(QWidget *parent = nullptr, QString codProd = "", QString descricao = "",
                                    int quant = 0, double valor = 0);
    ~FormEditarProdutoVenda();

    int quant;
    double valor;
    bool editado = false;

private slots:
    void on_confirmarButton_clicked();

    void on_cancelarButton_clicked();

private:
    Ui::FormEditarProdutoVenda *ui;
    QString codProd, descricao;
};

#endif // FORMEDITARPRODUTOVENDA_H
