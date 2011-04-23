#include "audiooutput.h"
#include "QMutex"

AudioOutput::AudioOutput(QAudioFormat format, QAudioDeviceInfo device, QList<QByteArray> * byteList) :
                   Audio(format, device, byteList, QAudio::AudioOutput)
{
    _audioThread = NULL;
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

    _byteList->clear();
    _byteArray = new QByteArray();

    init();
}

void AudioOutput::init()
{
    initOutput();
    _stream = new QDataStream(_byteArray, QIODevice::ReadOnly /*| QIODevice::Unbuffered*/);

    _ioDevice = _stream->device();

    _ioDevice->open(QIODevice::ReadWrite);
    _audioOutput->start(_ioDevice);

    _audioThread = new AudioOutputDataThread(_audioOutput, _ioDevice, _byteArray, _byteList);
    QObject::connect( _audioThread, SIGNAL( finished() ), this, SLOT( finishedThread() ) );

    _audioThread->start();
}

void AudioOutput::stop()
{
    if(_audioThread != NULL)
        _audioThread->stop();

    stopOutput();
}

void AudioOutput::finishedAudio(QAudio::State state)
{
    if(state == QAudio::StoppedState)
    {
        AudioOutput * sender = (AudioOutput *)QObject::sender();
        sender->disconnect();

        QDataStream * stream = sender->stream();

        delete sender;
        delete stream;

        emit finished();

        //qDebug("finishedAudio");
    }
}

void AudioOutput::finishedThread()
{
    AudioOutputDataThread * sender = (AudioOutputDataThread *)QObject::sender();
    sender->disconnect();

    if(sender->error())
    {
        init();
    }

    delete sender;

    //qDebug("finishedThread");
}

AudioOutputDataThread::AudioOutputDataThread(QAudioOutput * audioOutput, QIODevice * device, QByteArray * byteArray, QList<QByteArray> * byteList)
{
    _audioOutput = audioOutput;
    _byteArray   = byteArray;
    _byteList    = byteList;
    _device      = device;
    _exitThread  = false;
    _error       = false;
}

void AudioOutputDataThread::run()
{
    QMutex mutex;
    while(!_exitThread)
    {
        if(_audioOutput->error() != 0)
        {
            _error = true;
            break;
        }

        mutex.lock();
        if(!_byteList->isEmpty())
        {
            _byteArray->append(_byteList->takeFirst());
        }
        mutex.unlock();
    }

    return;

    exec();
}
