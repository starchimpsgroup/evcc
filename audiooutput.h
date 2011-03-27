#ifndef AUDIOOUTPUT_H
#define AUDIOOUTPUT_H

#include "audio.h"
#include "QThread"

class AudioOutputDataThread : public QThread
{
public:
    AudioOutputDataThread(QIODevice * device, QByteArray * byteArray, QVector<QByteArray> * byteVector);

    void run();
    void stop(){ _exitThread = true; }

private:
    bool                  _exitThread;
    QIODevice           * _device;
    QByteArray          * _byteArray;
    QVector<QByteArray> * _byteVector;
};

class AudioOutput : public Audio
{
public:
    AudioOutput(QAudioFormat format, QAudioDeviceInfo device, QVector<QByteArray> * byteVector);
    ~AudioOutput();

    void start();
    void stop();

private:
    AudioOutputDataThread * _audioThread;
    QDataStream * _stream;
    QIODevice   * _ioDevice;
    QByteArray  * _byteArray;

protected slots:
    void finishedAudio( QAudio::State state );
};

#endif // AUDIOOUTPUT_H
