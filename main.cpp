#include "formprincipal.h"

#include <QApplication>
#include <QSplashScreen>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon {":/imgs/imagens/logo32.png"});

    //Splash Screen
    QSplashScreen *spl = new QSplashScreen(QPixmap(":/imgs/imagens/logoGrande.png"));
    spl->show();
    QTimer::singleShot(2000, spl, &QSplashScreen::close);

    //Programa
    FormPrincipal w;
    QTimer::singleShot(2000, &w, &FormPrincipal::showMaximized);
    return a.exec();
}

/*
 * Gerar uma nota ao fechar a venda
*/

