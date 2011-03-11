#ifndef AUDIOOUTPUT_H
#define AUDIOOUTPUT_H

#include "audio.h"
#include "QThread"

class AudioOutputDataThread : public QThread
{
public:
    AudioOutputDataThread(QIODevice * device, QByteArray * byteArray);

    void run();
    void stop(){ _exitThread = true; }

private:
    bool          _exitThread;
    QIODevice    * _device;
    QByteArray   * _byteArray;
};

class AudioOutput : public Audio
{
public:
    AudioOutput(QAudioFormat format, QAudioDeviceInfo device, QByteArray * byteArray);
    ~AudioOutput();

    void start();
    void stop();

private:
    AudioOutputDataThread * _audioThread;
    QDataStream * _stream;
    QIODevice   * _ioDevice;

protected slots:
    void finishedAudio( QAudio::State state );
};

#endif // AUDIOOUTPUT_H
