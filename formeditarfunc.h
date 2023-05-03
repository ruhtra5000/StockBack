#ifndef FORMEDITARFUNC_H
#define FORMEDITARFUNC_H

#include <QDialog>
#include <QtSql>

namespace Ui {
class FormEditarFunc;
}

class FormEditarFunc : public QDialog
{
    Q_OBJECT

public:
    explicit FormEditarFunc(QWidget *parent = nullptr, QString id_func = 0);
    ~FormEditarFunc();

    bool editado = false;
private slots:
    void on_confirmarButton_clicked();

    void on_cancelarButton_clicked();

private:
    Ui::FormEditarFunc *ui;
    QString idFunc;
    QString usernamePadrao;
};

#endif // FORMEDITARFUNC_H
