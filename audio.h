#ifndef AUDIO_H
#define AUDIO_H

#include <QThread>
#include <QAudioInput>
#include <QAudioOutput>

class Audio : public QThread
{
    Q_OBJECT

public:
    Audio(QAudioFormat format, QAudioDeviceInfo device, QByteArray * byteArray, QAudio::Mode mode);

    virtual void run()       = 0;
    void stop(){ _exitThread = true; }


protected:
    bool             _exitThread;
    QAudioDeviceInfo _device;
    QAudioFormat     _format;
    QAudioInput    * _audioInput;
    QAudioOutput   * _audioOutput;
    QByteArray     * _byteArray;
};

#endif // AUDIO_H
