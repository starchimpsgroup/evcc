#ifndef LOADSPLASH_H
#define LOADSPLASH_H

#include <QObject>
#include <QSplashScreen>
#include <QTimerEvent>

//! The LoadSplash class.
/*!
    Animated splash screen.
 */
class LoadSplash : public QObject
{
    Q_OBJECT
public:

    //! Constructor LoadSplash class.
    /*!
        Load images
        \param screen Used splash screen
        \param interval Time interval of pictures
        \param parent Parent widget
     */
    explicit LoadSplash(QSplashScreen * screen, int interval, QObject *parent = 0);

    //! Start animation.
    void start();

private:
    //! Timer Event.
    /*!
        Show images
        \param event Timer event
     */
    void timerEvent(QTimerEvent *event);

private:
    QSplashScreen * _screen;   //!< Used splash screen
    QList<QPixmap>  _pixmaps;  //!< List of Pictures
    int             _interval; //!< Time interval of pictures
};

#endif // LOADSPLASH_H
