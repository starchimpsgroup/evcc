#ifndef AUDIO_H
#define AUDIO_H

#include <QAudioInput>
#include <QAudioOutput>
#include <QThread>
#include <QVector>
#include <QAudio>

class Audio : public QObject
{
    Q_OBJECT
public:
    Audio(QAudioFormat format, QAudioDeviceInfo device, QVector<QByteArray> * byteVector, QAudio::Mode mode);

    virtual void start() = 0;
    virtual void stop()  = 0;

    QString typ();

private:
    QAudio::Mode          _mode;

protected:
    void initInput();
    void initOutput();

    void stopInput();
    void stopOutput();

protected:
    QAudioDeviceInfo      _device;
    QAudioFormat          _format;
    QAudioInput         * _audioInput;
    QAudioOutput        * _audioOutput;
    QVector<QByteArray> * _byteVector;

protected slots:
    virtual void finishedThread();
    void finishedAudio( QAudio::State state );

signals:
    void finished();
    void audioData(QByteArray);
};

#endif // AUDIO_H
