#ifndef LOADSPLASH_H
#define LOADSPLASH_H

#include <QObject>
#include <QSplashScreen>
#include <QTimerEvent>

class LoadSplash : public QObject
{
    Q_OBJECT
public:
    explicit LoadSplash(QSplashScreen * screen, int interval, QObject *parent = 0);
    void start();

private:
    void timerEvent(QTimerEvent *event);

private:
    QSplashScreen * _screen;
    QList<QPixmap>  _pixmaps;
    int             _interval;

signals:

public slots:

};

#endif // LOADSPLASH_H
