#ifndef CLIENT_H
#define CLIENT_H

#include <QWidget>
#include <QAudioFormat>
#include "audioinput.h"
#include "audiooutput.h"
#include <QMap>

namespace Ui {
    class Client;
}

class Client : public QWidget
{
    Q_OBJECT

public:
    explicit Client(QWidget *parent = 0);
    ~Client();

    void stopAudioInput();
    void startAudioInput();

    void stopAudioOutput();
    void startAudioOutput();

    QAudioDeviceInfo audioDeviceByName( QString name, QAudio::Mode mode );

private:
    QAudioFormat _format;

    //QByteArray  * _audioInputByteArray;
    //QByteArray  * _audioOutputByteArray;
    QVector<QByteArray> * _audioInputVector;
    QVector<QByteArray> * _audioOutputVector;
    AudioInput  * _audioInput;
    AudioOutput * _audioOutput;

    //QMap<QObject *, QByteArray *> _audioMap;

private slots:
    void on_call_clicked();
    void on_endCall_clicked();
    //void finishedAudio();

private:
    Ui::Client *ui;
};

#endif // CLIENT_H
