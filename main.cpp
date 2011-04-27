#include <QtGui/QApplication>
#include <QSplashScreen>
#include <QTimer>
#include <QBitmap>
#include "loadsplash.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QPixmap pixmap0(":/logos/logoShow0.png");

    QSplashScreen splash(pixmap0);
    splash.setMask(pixmap0.mask());
    splash.show();

    LoadSplash  * loadSplash = new LoadSplash( &splash, 70 );
    loadSplash->start();

    MainWindow w;
    w.show();

    QTimer::singleShot(5000, &splash, SLOT(close()));

    return app.exec();
}
