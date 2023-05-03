#ifndef FORMEDITARPRODUTO_H
#define FORMEDITARPRODUTO_H

#include <QDialog>
#include <QtSql>

namespace Ui {
class FormEditarProduto;
}

class FormEditarProduto : public QDialog
{
    Q_OBJECT

public:
    explicit FormEditarProduto(QWidget *parent = nullptr, QString idProduto = "");
    ~FormEditarProduto();

    bool editado = false;
private slots:
    void on_confirmarButton_clicked();

    void on_cancelarButton_clicked();

private:
    Ui::FormEditarProduto *ui;
    QString idProd;
};

#endif // FORMEDITARPRODUTO_H
