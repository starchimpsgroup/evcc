#include "audioinput.h"
#include <QMutex>

AudioInput::AudioInput(QAudioFormat format, QAudioDeviceInfo device, QVector<QByteArray> * byteVector) :
                 Audio(format, device, byteVector, QAudio::AudioInput)
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
    /*_ioDevice = new QBuffer();

    if (!_ioDevice->open(QIODevice::ReadWrite))
        qDebug("unable to open buffer");*/

    _ioDevice = _audioInput->start();

    //Q_ASSERT(_ioDevice);
    //QObject::connect(_ioDevice, SIGNAL(readyRead()), this, SLOT(readAudioData()));

    _audioThread = new AudioInputDataThread(_audioInput, _ioDevice, _byteVector);
    _audioThread->setParent(this);
    QObject::connect( _audioThread, SIGNAL( finished() ), this, SLOT( finishedThread() ) );

    _audioThread->start();
}

/*void AudioInput::readAudioData()
{
    qDebug("test");
    QBuffer * sender = (QBuffer *)QObject::sender();
    QByteArray appendData;

    if( _audioInput->bytesReady() >= _audioInput->periodSize() )
    {
        appendData = sender->read(_audioInput->periodSize());
        //sender->reset();
        //sender->seek(0);

        //sender->read(data, _audioInput->periodSize());
        //appendData = data;

        _byteVector->append(appendData);

        qDebug(qPrintable("Packets: " + QString::number(_byteVector->size())));
    }
    qDebug(qPrintable("BytesReady: " + QString::number(_audioInput->bytesReady())));
}*/

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

    qDebug("finishedThread");
}

AudioInputDataThread::AudioInputDataThread(QAudioInput * audioInput, QIODevice * device, QVector<QByteArray> * byteVector)
{
    _audioInput = audioInput;
    _device     = device;
    _byteVector = byteVector;
    _exitThread = false;
}

void AudioInputDataThread::run()
{
    QByteArray appendData;
    //char * data;
    QMutex mutex;
    while(!_exitThread)
    {
        if(_audioInput->error() != 0)
            qDebug(qPrintable("Input " + QString::number(_audioInput->error())));

        //qDebug(qPrintable("BytesReady: " + QString::number(_audioInput->bytesReady())));
        //qDebug(qPrintable("PeriodSize: " + QString::number(_audioInput->periodSize())));
        //qDebug(qPrintable("BufferSize: " + QString::number(_audioInput->bufferSize())));

        mutex.lock();
        if( _audioInput->bytesReady() >= _audioInput->periodSize() )
        {
            //_device->seek(0);
            appendData = _device->read(_audioInput->periodSize());
            //_device->reset();
            _device->seek(0);

            //_device->read(data, _audioInput->bytesReady()/*_audioInput->periodSize()*/);
            //appendData = data;

            _byteVector->append(appendData);

            //qDebug(qPrintable("Packets: " + QString::number(_byteVector->size())));
        }
        mutex.unlock();
    }

    return;

    exec();
}
