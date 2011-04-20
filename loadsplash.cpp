#include "loadsplash.h"

LoadSplash::LoadSplash(QSplashScreen * screen, int interval, QObject *parent) :
    QObject(parent)
{
    _screen   = screen;
    _interval = interval;

    _pixmaps.append(QPixmap (":/logos/logoShow1.png"));
    _pixmaps.append(QPixmap (":/logos/logoShow2.png"));
    _pixmaps.append(QPixmap (":/logos/logoShow3.png"));
    _pixmaps.append(QPixmap (":/logos/logoShow4.png"));
    _pixmaps.append(QPixmap (":/logos/logoShow5.png"));
    _pixmaps.append(QPixmap (":/logos/logoShow6.png"));
    _pixmaps.append(QPixmap (":/logos/logoShow7.png"));
}

void LoadSplash::start()
{
    startTimer(_interval);
}

void LoadSplash::timerEvent(QTimerEvent *event)
{
    if(!_pixmaps.isEmpty())
    {
        _screen->setPixmap(_pixmaps.takeFirst());
    }
    else
    {
        killTimer(event->timerId());
    }
}
