#ifndef FORMGESTAOFORNECEDORES_H
#define FORMGESTAOFORNECEDORES_H

#include "Conexao.h"

#include <QDialog>
#include <QtSql>
#include <QStatusBar>

namespace Ui {
class FormGestaoFornecedores;
}

class FormGestaoFornecedores : public QDialog
{
    Q_OBJECT

public:
    explicit FormGestaoFornecedores(QWidget *parent = nullptr);
    ~FormGestaoFornecedores();

private slots:
    //Fornecedores - GESTÃO
    void atualizarFornecedores(QString query, QString msgErro);

    void on_editarButton_clicked();

    void on_deletarButton_clicked();

    void on_filtrarButton_clicked();

    void on_idRadioButton_clicked();

    void on_nomeRadioButton_clicked();

    //Fornecedores - ADICIONAR
    void on_criarButton_clicked();

    void on_limparButton_clicked();

    //Fornecedores - REGISTROS
    void atualizarRegistros(QString query, QString msgErro);

    void on_registrarAqButton_clicked();

    void on_deletarAqButton_clicked();

    void on_filtrarAqButton_clicked();

    void on_restaurarButton_clicked();

private:
    Ui::FormGestaoFornecedores *ui;
    Conexao con;
    QStatusBar *bar;
};

#endif // FORMGESTAOFORNECEDORES_H
