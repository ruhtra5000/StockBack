#ifndef FORMPESQUISARAQUISICAO_H
#define FORMPESQUISARAQUISICAO_H

#include <QDialog>
#include <QtSql>

namespace Ui {
class FormPesquisarAquisicao;
}

class FormPesquisarAquisicao : public QDialog
{
    Q_OBJECT

public:
    explicit FormPesquisarAquisicao(QWidget *parent = nullptr, int tipo = 0);
    ~FormPesquisarAquisicao();

    QString codSelec = "0";
private slots:
    void carregarDados(QString query, int tipo);

    void on_selecionarButton_clicked();

private:
    Ui::FormPesquisarAquisicao *ui;
    QStringList forn {"Cód. Forn.", "Nome", "CNPJ"};
    QStringList prod {"Cód. Prod.", "Produto", "Estoque"};
};

#endif // FORMPESQUISARAQUISICAO_H
