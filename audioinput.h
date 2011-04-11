#ifndef AUDIOINPUT_H
#define AUDIOINPUT_H

#include "audio.h"
#include <QThread>

class AudioInputDataThread : public QThread
{
public:
    AudioInputDataThread(QAudioInput * audioInput, QIODevice * device, QVector<QByteArray> * byteVector);

    void run();
    void stop(){ _exitThread = true; }

private:
    bool                  _exitThread;
    QAudioInput         * _audioInput;
    QIODevice           * _device;
    QVector<QByteArray> * _byteVector;

};

class AudioInput : public Audio
{
public:
    AudioInput(QAudioFormat format, QAudioDeviceInfo device, QVector<QByteArray> * byteVector);
    ~AudioInput();

    void start();
    void stop();

private:
    AudioInputDataThread * _audioThread;
    QIODevice            * _ioDevice;

protected slots:
    //void readAudioData();
    void finishedThread();
};

#endif // AUDIOINPUT_H
