#include "audiooutput.h"

AudioOutput::AudioOutput(QAudioFormat format, QAudioDeviceInfo device, QByteArray * byteArray) :
                   Audio(format, device, byteArray, QAudio::AudioOutput)
{
}

AudioOutput::~AudioOutput()
{
    stop();
}

void AudioOutput::start()
{
    if (!_device.isFormatSupported(_format)) {
        qDebug("default format not supported try to use nearest");
        _format = _device.nearestFormat(_format);
    }

    _byteArray->clear();
    _stream = new QDataStream(_byteArray, QIODevice::ReadWrite /*| QIODevice::Unbuffered*/);

    _ioDevice = _stream->device();

     _ioDevice->open(QIODevice::ReadWrite);
    _audioOutput->start(_ioDevice);
}

void AudioOutput::stop()
{
    _audioOutput->stop();
}

void AudioOutput::finishedAudio(QAudio::State state)
{
    if(state == QAudio::StoppedState)
    {qDebug("finishedAudio");
        QObject * sender = QObject::sender();
        sender->disconnect();

        delete _stream;

        emit finished();
    }
}
