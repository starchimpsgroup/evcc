#include "audioinput.h"

AudioInput::AudioInput(QAudioFormat format, QAudioDeviceInfo device, QByteArray * byteArray) :
            Audio(format, device, byteArray, QAudio::AudioInput)
{

}

void AudioInput::run()
{
    if (!_device.isFormatSupported(_format)) {
        qDebug("default format not supported try to use nearest");
        _format = _device.nearestFormat(_format);
    }

    QIODevice * ioDevice;
//int i = 0; // ####
    ioDevice = _audioInput->start();
//qDebug(qPrintable(QString::number(_audioInput->bufferSize()))); // ####
//qDebug(qPrintable(QString::number(_audioInput->periodSize()))); // ####
    while(!_exitThread)
    {
        if( _audioInput->bytesReady() >= _audioInput->periodSize() )
        {
            //i++; // ####
            *_byteArray += ioDevice->read(_audioInput->bytesReady());
            //qDebug(qPrintable(QString::number(i) + " :: len: " + QString::number(_audioInput->bytesReady()))); // ####
        }
    }

    _audioInput->stop();

    qDebug(qPrintable(QString::number(_byteArray->size()))); // ####

    delete _audioInput;
    delete ioDevice;

    return;

    exec();
}
