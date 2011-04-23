#ifndef CLIENT_H
#define CLIENT_H

#include <QWidget>
#include <QAudioFormat>
#include <QMap>
#include "audioinput.h"
#include "audiooutput.h"
#include "clientconnection.h"

class OutputDataThread : public QThread
{
public:
    OutputDataThread(ClientConnection * connection, QList<QByteArray> * byteList);

    void run();
    void stop() { _exitThread = true; }

private:
    bool                _exitThread;
    QList<QByteArray> * _byteList;
    ClientConnection  * _connection;
};

namespace Ui {
    class Client;
}

class Client : public QWidget
{
    Q_OBJECT

public:
    explicit Client(QString server, quint16 port, QString userName, QWidget *parent = 0);
    ~Client();

    void stopAudioInput();
    void startAudioInput();

    void stopAudioOutput();
    void startAudioOutput();

    QAudioDeviceInfo audioDeviceByName( QString name, QAudio::Mode mode );
    QAudioFormat format(){ return _format; }

private:
    Ui::Client          * ui;

    QAudioFormat          _format;

    QList<QByteArray>   * _audioInputList;
    QList<QByteArray>   * _audioOutputList;
    AudioInput          * _audioInput;
    AudioOutput         * _audioOutput;

    ClientConnection    * _connection;
    QString               _server;
    quint16               _port;

    bool                  _callEstablished;

    OutputDataThread    * _dataThread;

private slots:
    void on_call_clicked();
    void on_endCall_clicked();
    void showMessage(QString, ServerMessages::MessageTyp);
    void userListRefresh();
    void callEstablished();
    void callTerminated();
    void receivedSoundData(QByteArray);

signals:
    void serverError();
    void setStatusBarText(QString);

protected slots:
    virtual void finishedThread();
};

#endif // CLIENT_H
