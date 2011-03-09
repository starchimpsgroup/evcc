#include "audioinput.h"

AudioInput::AudioInput(QAudioFormat format, QAudioDeviceInfo device, QByteArray * byteArray) :
                 Audio(format, device, byteArray, QAudio::AudioInput)
{
}

AudioInput::~AudioInput()
{
    stop();
}

void AudioInput::start()
{
    if (!_device.isFormatSupported(_format)) {
        qDebug("default format not supported try to use nearest");
        _format = _device.nearestFormat(_format);
    }

    _ioDevice = _audioInput->start();

    _audioThread = new AudioInputDataThread(_audioInput, _ioDevice, _byteArray);
    QObject::connect( _audioThread, SIGNAL( finished() ), this, SLOT( finishedThread() ) );

    _audioThread->start();

    // ##################

    /*QList<QAudioDeviceInfo> i = QAudioDeviceInfo::availableDevices(QAudio::AudioInput);
    QList<QAudioDeviceInfo> o = QAudioDeviceInfo::availableDevices(QAudio::AudioOutput);

    QAudioDeviceInfo outputDevice = _device;//QAudioDeviceInfo::defaultOutputDevice();
    QAudioDeviceInfo inputDevice  = _device;//QAudioDeviceInfo::defaultInputDevice();

    QAudioInput    * _audioInput;
    QAudioOutput   * _audioOutput;

    _audioOutput = new QAudioOutput(outputDevice, _format);
    _audioInput  = new QAudioInput(inputDevice, _format);

    //QIODevice * ioDevice;
    _ioDevice = _audioInput->start();
    _audioOutput->start(_ioDevice);*/

    // ##################
}

void AudioInput::stop()
{
    _audioThread->stop();
}

void AudioInput::finishedThread()
{
    QObject * sender = QObject::sender();
    sender->disconnect();
    delete sender;

    _audioInput->stop();

    qDebug("finishedThread");
}

AudioInputDataThread::AudioInputDataThread(QAudioInput * audioInput, QIODevice * device, QByteArray * byteArray)
{
    _audioInput = audioInput;
    _device     = device;
    _byteArray  = byteArray;
    _exitThread = false;
}

void AudioInputDataThread::run()
{
    while(!_exitThread)
    {
        if( _audioInput->bytesReady() >= _audioInput->periodSize() )
        {
            _byteArray->append( _device->read(_audioInput->bytesReady()) );
        }
    }

    qDebug(qPrintable("Bytes read: " + QString::number(_byteArray->size()))); // ####

    return;

    exec();
}
