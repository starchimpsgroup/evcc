#include "audiooutput.h"

AudioOutput::AudioOutput(QAudioFormat format, QAudioDeviceInfo device, QByteArray * byteArray) :
                   Audio(format, device, byteArray, QAudio::AudioOutput)
{
}

void AudioOutput::run()
{
    if (!_device.isFormatSupported(_format)) {
        qDebug("default format not supported try to use nearest");
        _format = _device.nearestFormat(_format);
    }

    QDataStream * stream = new QDataStream(_byteArray, QIODevice::ReadWrite | QIODevice::Unbuffered);

    _audioOutput->start(stream->device());

    while(!_exitThread)
    {
    }

    _audioOutput->stop();

    delete _audioOutput;
    delete stream;

    return;

    exec();
}
