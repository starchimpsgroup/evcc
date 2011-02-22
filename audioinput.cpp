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
    ioDevice = _audioInput->start();

    // ##################

    /*QList<QAudioDeviceInfo> i = QAudioDeviceInfo::availableDevices(QAudio::AudioInput);
    QList<QAudioDeviceInfo> o = QAudioDeviceInfo::availableDevices(QAudio::AudioOutput);

    QAudioDeviceInfo outputDevice = i.at(0);//QAudioDeviceInfo::defaultOutputDevice();
    QAudioDeviceInfo inputDevice  = i.at(1);//QAudioDeviceInfo::defaultInputDevice();

    QAudioInput    * _audioInput;
    QAudioOutput   * _audioOutput;

    _audioOutput = new QAudioOutput(outputDevice, _format);
    _audioInput  = new QAudioInput(inputDevice, _format);

    //QIODevice * ioDevice;
    ioDevice = _audioInput->start();
    _audioOutput->start(ioDevice);*/

    // ##################

    while(!_exitThread)
    {
        /*if( _audioInput->bytesReady() >= _audioInput->periodSize() )
        {
            //_byteArray->append( ioDevice->read(_audioInput->bytesReady()) );
        }*/
    }

    _audioInput->stop();

    qDebug(qPrintable("Bytes read: " + QString::number(_byteArray->size()))); // ####

    delete _audioInput;
    delete ioDevice;

    return;

    exec();
}
