#ifndef FORMGESTAOFUNC_H
#define FORMGESTAOFUNC_H

#include "Conexao.h"

#include <QDialog>
#include <QtSql>
#include <QStatusBar>

namespace Ui {
class FormGestaoFunc;
}

class FormGestaoFunc : public QDialog
{
    Q_OBJECT

public:
    explicit FormGestaoFunc(QWidget *parent = nullptr);
    ~FormGestaoFunc();
private slots:
    //Funcionarios - GESTÃO
    void atualizarFunc(QString query, QString msgErro);

    void on_editarButton_clicked();

    void on_deletarButton_clicked();

    void on_filtrarButton_clicked();

    void on_idRadioButton_clicked();

    void on_nomeRadioButton_clicked();

    //Funcionarios - ADICIONAR
    void on_criarButton_clicked();

    void on_limparButton_clicked();

private:
    Ui::FormGestaoFunc *ui;
    Conexao con;
    QStatusBar *bar;
};

#endif // FORMGESTAOFUNC_H
