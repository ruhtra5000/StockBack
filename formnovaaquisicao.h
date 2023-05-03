#ifndef FORMNOVAAQUISICAO_H
#define FORMNOVAAQUISICAO_H

#include <QDialog>
#include <QtSql>

namespace Ui {
class FormNovaAquisicao;
}

class FormNovaAquisicao : public QDialog
{
    Q_OBJECT

public:
    explicit FormNovaAquisicao(QWidget *parent = nullptr);
    ~FormNovaAquisicao();

    bool criado = false;
private slots:
    void on_registrarButton_clicked();

    void on_pesquisarFornButton_clicked();

    void on_pesquisarProdButton_clicked();

    void on_dataCheckBox_stateChanged(int arg1);

    void on_horaCheckBox_stateChanged(int arg1);

private:
    Ui::FormNovaAquisicao *ui;
};

#endif // FORMNOVAAQUISICAO_H
