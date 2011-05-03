#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "choice.h"
#include "client.h"
#include "server.h"
#include "playfile.h"
#include "preferences.h"
#include "settings.h"

namespace Ui {
    class MainWindow;
}

//! The MainWindow class.
/*!
    Main window screen.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    //! Constructor MainWindow class.
    /*!
        Load sounds
        \param parent Parent widget
     */
    explicit MainWindow(QWidget *parent = 0);

    //! Destructor MainWindow class.
    ~MainWindow();

private:
    //! Set choice window.
    /*!
        Set the window back to the choice, it is used aufter an error.
     */
    void setChoice();

private:
    Ui::MainWindow * ui;           //!< The UI

    Choice         * _choice;      //!< Choice window
    Client         * _client;      //!< Client window
    Server         * _server;      //!< Server window

    PlayFile         _playRing;    //!< Sound, if user is calling someone
    PlayFile         _playRingPH;  //!< Sound, if user were called
    PlayFile         _playOffHook; //!< Sound, if call denied

    Preferences    * _preferences; //!< Preferences window
    Settings         _settings;    //!< Programm settings

private slots:
    //! Set page.
    /*!
        Show chosen page, client or server
        \see ContentPage
        \param page Page content
     */
    void setPage(Choice::ContentPage page);

    //! Server Error.
    /*!
        Called if an client error occurred, to show the choice window again
        and delete the client
        \see Client
        \see Choice
     */
    void serverError();

    //! Connection established.
    /*!
        Shows client window
        \see Client
     */
    void connectionEstablished();

    //! Stop sound.
    /*!
        Stops all audio sounds
        \see PlayFile
     */
    void stopSound();

    //! Call out.
    /*!
        User is calling someone\n
        Plays calling sound
        \see PlayFile
        \param name Call out username
     */
    void callOut(QString name);

    //! Call in.
    /*!
        Someone is calling the user\n
        Plays calling sound
        \see PlayFile
        \param name Call in username
     */
    void callIn(QString name);

    //! Call denied.
    /*!
        User is calling someone, but callingpartner is currently calling with someone else\n
        Plays calling denied sound
        \see PlayFile
        \param Call denied username
     */
    void callDenied(QString name);

    //! Call terminated.
    /*!
        The callingpartner has terminated the call ore the server is down
        Stops all sounds
        \see PlayFile
     */
    void callTerminated();

    //! Call terminated.
    /*!
        The callingpartner has accept the call out
        Stops all sounds
        \see PlayFile
        \see callOut
     */
    void callEstablished();

    void on_actionPreferences_triggered();
};

#endif // MAINWINDOW_H
