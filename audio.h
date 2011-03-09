#ifndef AUDIO_H
#define AUDIO_H

#include <QAudioInput>
#include <QAudioOutput>

class Audio : public QObject
{
    Q_OBJECT
public:
    Audio(QAudioFormat format, QAudioDeviceInfo device, QByteArray * byteArray, QAudio::Mode mode);

    virtual void start() = 0;
    virtual void stop()  = 0;

protected:
    QAudioDeviceInfo _device;
    QAudioFormat     _format;
    QAudioInput    * _audioInput;
    QAudioOutput   * _audioOutput;
    QByteArray     * _byteArray;

protected slots:
    virtual void finishedThread(){}
    void finishedAudio( QAudio::State state );

signals:
    void finished();
};

#endif // AUDIO_H
