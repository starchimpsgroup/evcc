#ifndef CLIENT_H
#define CLIENT_H

#include <QWidget>
#include <QAudioFormat>
#include "audioinput.h"
#include "audiooutput.h"
#include <QMap>
#include "clientconnection.h"

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

private:
    QAudioFormat _format;

    QVector<QByteArray> * _audioInputVector;
    QVector<QByteArray> * _audioOutputVector;
    AudioInput          * _audioInput;
    AudioOutput         * _audioOutput;

    ClientConnection    * _connection;
    QString               _server;
    quint16               _port;

private slots:
    void on_call_clicked();
    void on_endCall_clicked();
    void showMessage(QString, ServerMessages::MessageTyp);

private:
    Ui::Client *ui;
};

#endif // CLIENT_H
