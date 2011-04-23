#include "audioinput.h"
#include <QMutex>

AudioInput::AudioInput(QAudioFormat format, QAudioDeviceInfo device, QList<QByteArray> * byteList) :
                 Audio(format, device, byteList, QAudio::AudioInput)
{
    _audioThread = NULL;
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

    initInput();

    _ioDevice = _audioInput->start();

    _audioThread = new AudioInputDataThread(_audioInput, _ioDevice, _byteList);
    _audioThread->setParent(this);
    QObject::connect( _audioThread, SIGNAL( finished() ), this, SLOT( finishedThread() ) );

    _audioThread->start();
}

void AudioInput::stop()
{
    if(_audioThread != NULL)
    {
        _audioThread->stop();
        _audioThread = NULL;
    }
}

void AudioInput::finishedThread()
{
    AudioInputDataThread * sender = (AudioInputDataThread *)QObject::sender();
    sender->disconnect();

    AudioInput * parent = (AudioInput *)sender->parent();
    parent->stopInput();

    delete sender;

    //qDebug("finishedThread");
}

AudioInputDataThread::AudioInputDataThread(QAudioInput * audioInput, QIODevice * device, QList<QByteArray> * byteList)
{
    _audioInput = audioInput;
    _device     = device;
    _byteList   = byteList;
    _exitThread = false;
}

void AudioInputDataThread::run()
{
    QByteArray appendData;
    QMutex mutex;
    while(!_exitThread)
    {
        if(_audioInput->error() != 0)
            qDebug(qPrintable("InputError " + QString::number(_audioInput->error())));

        mutex.lock();
        if( _audioInput->bytesReady() >= _audioInput->periodSize() )
        {
            appendData = _device->read(_audioInput->periodSize());
            _device->seek(0);

            _byteList->append(appendData);

        }
        mutex.unlock();
    }

    return;

    exec();
}
