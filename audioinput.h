#ifndef AUDIOINPUT_H
#define AUDIOINPUT_H

#include "audio.h"
#include <QThread>

class AudioInputDataThread : public QThread
{
public:
    AudioInputDataThread(QAudioInput * audioInput, QIODevice * device, QList<QByteArray> * byteList);

    void run();
    void stop(){ _exitThread = true; }

private:
    bool                  _exitThread;
    QAudioInput         * _audioInput;
    QIODevice           * _device;
    QList<QByteArray>   * _byteList;

};

class AudioInput : public Audio
{
public:
    AudioInput(QAudioFormat format, QAudioDeviceInfo device, QList<QByteArray> * byteList);
    ~AudioInput();

    void start();
    void stop();

private:
    AudioInputDataThread * _audioThread;
    QIODevice            * _ioDevice;

protected slots:
    void finishedThread();
};

#endif // AUDIOINPUT_H
