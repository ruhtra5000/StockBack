#ifndef FORMEDITARFORNECEDOR_H
#define FORMEDITARFORNECEDOR_H

#include <QDialog>
#include <QtSql>

namespace Ui {
class FormEditarFornecedor;
}

class FormEditarFornecedor : public QDialog
{
    Q_OBJECT

public:
    explicit FormEditarFornecedor(QWidget *parent = nullptr, QString idForn = "");
    ~FormEditarFornecedor();

    bool editado = false;
private slots:
    void on_confirmarButton_clicked();

    void on_cancelarButton_clicked();

private:
    Ui::FormEditarFornecedor *ui;
    QString codForn;
};

#endif // FORMEDITARFORNECEDOR_H
