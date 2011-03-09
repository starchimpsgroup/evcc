#include "audio.h"

Audio::Audio(QAudioFormat format, QAudioDeviceInfo device, QByteArray * byteArray, QAudio::Mode mode)
{
    _format      = format;
    _device      = device;

    qDebug(qPrintable(device.deviceName()));

    if(mode == QAudio::AudioInput)
    {
        _audioInput   = new QAudioInput(_device, _format);
        QObject::connect(_audioInput, SIGNAL( stateChanged ( QAudio::State ) ), this, SLOT( finishedAudio( QAudio::State ) ) );
    }

    if(mode == QAudio::AudioOutput)
    {
        _audioOutput  = new QAudioOutput(_device, _format);
        QObject::connect(_audioOutput, SIGNAL( stateChanged ( QAudio::State ) ), this, SLOT( finishedAudio( QAudio::State ) ) );
    }

    _byteArray  = byteArray;
}

void Audio::finishedAudio(QAudio::State state)
{
    if(state == QAudio::StoppedState)
    {qDebug("finishedAudio");
        QObject * sender = QObject::sender();
        sender->disconnect();

        //delete sender;

        emit finished();
    }
}
