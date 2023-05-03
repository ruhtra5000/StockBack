#ifndef FORMGESTAOVENDAS_H
#define FORMGESTAOVENDAS_H

#include "Conexao.h"

#include <QDialog>
#include <QtSql>
#include <QStatusBar>

namespace Ui {
class FormGestaoVendas;
}

class FormGestaoVendas : public QDialog
{
    Q_OBJECT

public:
    explicit FormGestaoVendas(QWidget *parent = nullptr);
    ~FormGestaoVendas();

private slots:
    void atualizarVendas(QString query, QString msgErro);

    void atualizarProdutosVendas(QString idVenda);

    void on_tabelaVendas_itemSelectionChanged();

    void on_filtrarButton_clicked();

    void on_restaurarButton_clicked();

private:
    Ui::FormGestaoVendas *ui;
    Conexao con;
    QStatusBar *bar;
};

#endif // FORMGESTAOVENDAS_H
