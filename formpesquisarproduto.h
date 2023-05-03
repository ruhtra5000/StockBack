#ifndef FORMPESQUISARPRODUTO_H
#define FORMPESQUISARPRODUTO_H

#include <QDialog>
#include <QtSql>

namespace Ui {
class FormPesquisarProduto;
}

class FormPesquisarProduto : public QDialog
{
    Q_OBJECT

public:
    explicit FormPesquisarProduto(QWidget *parent = nullptr);
    ~FormPesquisarProduto();

    bool selec = false;
    int codProd = 0;
private slots:
    void on_selecionarButton_clicked();

    void on_cancelarButton_clicked();

private:
    Ui::FormPesquisarProduto *ui;
};

#endif // FORMPESQUISARPRODUTO_H
