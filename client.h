#ifndef CLIENT_H
#define CLIENT_H

#include <QWidget>
#include <QAudioFormat>
#include <QMap>
#include "audioinput.h"
#include "audiooutput.h"
#include "clientconnection.h"

//! Thread for sending the parts of the audiopart list
/*!
    Tackes the parts from the audio list for sending.
 */
class OutputDataThread : public QThread
{
public:
    //! Constructor AudioOutputDataThread.
    /*!
        \param connection Client connection
        \param byteList List of readed audio packets
     */
    OutputDataThread(ClientConnection * connection, QList<QByteArray> * byteList);

    //! Starts the thread activity.
    /*!
        Takes bytes from the input list and send them to the callingpartner.
     */
    void run();

    //! Stops thread.
    /*!
        Sets the exit variable.
     */
    void stop()    { _exitThread  = true; }

    //! Send data counter.
    /*!
        The counter for sendet packets.
     */
    void sendData(){ _sendData++; }

private:
    bool                _exitThread; //!< Exit variable
    int                 _sendData;   //!< Counter for sendet packets
    QList<QByteArray> * _byteList;   //!< Audio part list
    ClientConnection  * _connection; //!< Client connection
};

namespace Ui {
    class Client;
}

//! The Client class.
/*!
    Client to call other clients.
 */
class Client : public QWidget
{
    Q_OBJECT

public:
    //! Constructor Client class.
    /*!
        Initialize connection to the Server, connect singals and slots, set audio format.
        \param server Server name or ip
        \param port Server port
        \param inputDeviceName Name of audio input device
        \param outputDeviceName Name of audio output device
        \param userName Client username
        \param parent Parent widget
     */
    explicit Client(QString server, quint16 port, QString inputDeviceName, QString outputDeviceName, QString userName, QWidget *parent = 0);

    //! Destructor Client class.
    /*!
        Terminate call
     */
    ~Client();

    //! Stop audio input.
    /*!
        Stops audio input and clears input list
     */
    void stopAudioInput();

    //! Start audio input.
    /*!
        Starts audio input and clears input list
     */
    void startAudioInput();

    //! Stop audio output.
    /*!
        Stops audio output and clears input list
     */
    void stopAudioOutput();

    //! Start audio output.
    /*!
        Starts audio output and clears input list
     */
    void startAudioOutput();

    //! Audio device by name.
    /*!
        Returns the audio device by a specific name and mode
        \return Named audio device
        \param name Name of audio device
        \param mode Mode input/output
     */
    QAudioDeviceInfo audioDeviceByName( QString name, QAudio::Mode mode );

    //! Audio format.
    /*!
        The format of the audio
        \return Audio format
     */
    QAudioFormat format(){ return _format; }

private:
    Ui::Client          * ui;               //!< The UI

    QAudioFormat          _format;          //!< Specified format of the audio data

    QList<QByteArray>   * _audioInputList;  //!< List of audio input
    QList<QByteArray>   * _audioOutputList; //!< List of audio output
    AudioInput          * _audioInput;      //!< Audio input
    AudioOutput         * _audioOutput;     //!< Audio output

    ClientConnection    * _connection;      //!< Client connection
    QString               _server;          //!< Serveradress or ip
    quint16               _port;            //!< Server port number

    bool                  _callEstablished; //!< Call established value

    OutputDataThread    * _dataThread;      //!< Data thread

private slots:

    //! Call button.
    /*!
        Accept incomming call or make new call
     */
    void on_call_clicked();

    //! Call end button.
    /*!
        Terminates incomming or active call
     */
    void on_endCall_clicked();

    //! Show Message.
    /*!
        Shows messagewindow with text and specified typ
        \see ServerMessages
        \param text Shown text
        \param typ Typ of text
     */
    void showMessage(QString, ServerMessages::MessageTyp);

    //! User list refresh.
    /*!
        Shows current user list
     */
    void userListRefresh();

    //! Call established.
    /*!
        Starts audio input, output and a new data thread
     */
    void callEstablished();

    //! Call terminated.
    /*!
        Stops audio input, output and the data thread
     */
    void callTerminated();

    //! Received Sound Data.
    /*!
        Stops audio input, output and the data thread
        \param date Received sound data
     */
    void receivedSoundData(QByteArray);

    //! Data transferred.
    /*!
        Data counter for sendet packets
     */
    void dataTransferred();

signals:
    //! Server error.
    /*!
        Fatal error, client can not work
     */
    void serverError();

    //! Set status bar text.
    /*!
        New text for the status bar
        \param text Text of statusbar
     */
    void setStatusBarText(QString);

protected slots:
    //! Called by finished signal from thread.
    /*!
        Disconnect and delete thread.
     */
    virtual void finishedThread();
};

#endif // CLIENT_H
