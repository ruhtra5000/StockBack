#ifndef FORMNOVAVENDA_H
#define FORMNOVAVENDA_H

#include "Conexao.h"

#include <QDialog>
#include <QtSql>
#include <QStatusBar>

namespace Ui {
class FormNovaVenda;
}

class FormNovaVenda : public QDialog
{
    Q_OBJECT

public:
    explicit FormNovaVenda(QWidget *parent = nullptr);
    ~FormNovaVenda();

    bool vendaAberta = false;
private slots:
    void on_adicionarButton_clicked();

    void on_pesquisarButton_clicked();

    void on_editarButton_clicked();

    void on_deletarButton_clicked();

    void on_finalizarButton_clicked();

    void on_finalizarFecharButton_clicked();

    void gerarNota(QString id);

    bool estaNaVenda(QString codigo);

    void calcularValorTotal();

private:
    Ui::FormNovaVenda *ui;
    Conexao con;
    QStatusBar *bar;
    int linha = 0;
    int fechar = 0;
};

#endif // FORMNOVAVENDA_H
