#ifndef AUDIOOUTPUT_H
#define AUDIOOUTPUT_H

#include "audio.h"

class AudioOutput : public Audio
{
public:
    AudioOutput(QAudioFormat format, QAudioDeviceInfo device, QByteArray * byteArray);
    ~AudioOutput();

    void start();
    void stop();

private:
    QDataStream * _stream;
    QIODevice   * _ioDevice;

protected slots:
    void finishedAudio( QAudio::State state );
};

#endif // AUDIOOUTPUT_H
