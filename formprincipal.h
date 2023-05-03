#ifndef FORMPRINCIPAL_H
#define FORMPRINCIPAL_H

#include "Conexao.h"

#include <QMainWindow>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui { class FormPrincipal; }
QT_END_NAMESPACE

class FormPrincipal : public QMainWindow
{
    Q_OBJECT

public:
    FormPrincipal(QWidget *parent = nullptr);
    ~FormPrincipal();

    static int idFunc;
    static QString nomeFunc, acessoFunc;

    QIcon fecharIcon {":/imgs/imagens/fecharVenda.png"};
    QIcon delIcon {":/imgs/imagens/deletar.png"};
signals:
    void fecharSignal(QString id);
    void deletarSignal(QString id);

private slots:
    //Tela Principal
    void limparVendas();

    void atualizarVendasAbertas(bool limpar);

    void on_novaVendaButton_clicked();

    void fecharVenda(QString id);

    void deletarVenda(QString id);

    void gerarNota(QString id);

    //Menu superior - GESTÃO
    void on_actionEstoque_triggered();

    void on_actionFuncion_rios_triggered();

    void on_actionVendas_triggered();

    void on_actionFornecedores_triggered();

    bool adminLogado();

    //Menu superior - SISTEMA
    void on_actionSair_triggered();

    void on_actionStockBack_triggered();

    void on_actionSobre_o_Qt_Creator_triggered();

    //Toolbar
    void on_actionLogin_triggered();

private:
    Ui::FormPrincipal *ui;
    Conexao con;
    QLabel *nomeUsuario;
    bool vendaAberta;
};
#endif // FORMPRINCIPAL_H
