#ifndef AUDIOOUTPUT_H
#define AUDIOOUTPUT_H

#include "audio.h"
#include "QThread"

class AudioOutputDataThread : public QThread
{
public:
    AudioOutputDataThread(QAudioOutput * audioOutput, QIODevice * device, QByteArray * byteArray, QList<QByteArray> * byteList);

    void run();
    void stop() { _exitThread = true; }

    bool error(){ return _error; }

private:
    bool                  _exitThread;
    bool                  _error;
    QAudioOutput        * _audioOutput;
    QIODevice           * _device;
    QByteArray          * _byteArray;
    QList<QByteArray>   * _byteList;
};

class AudioOutput : public Audio
{
public:
    AudioOutput(QAudioFormat format, QAudioDeviceInfo device, QList<QByteArray> * byteList);
    ~AudioOutput();

    void start();
    void stop();

    QDataStream * stream(){ return _stream; }
    QByteArray  * byteArray(){ return _byteArray; }

private:
    void init();

private:
    AudioOutputDataThread * _audioThread;
    QDataStream           * _stream;
    QIODevice             * _ioDevice;
    QByteArray            * _byteArray;

protected slots:
    virtual void finishedThread();
    void finishedAudio( QAudio::State state );
};

#endif // AUDIOOUTPUT_H
