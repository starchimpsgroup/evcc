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
    _ioDevice = _audioInput->start();

    _audioThread = new AudioInputDataThread(_audioInput, _ioDevice, _byteVector);
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

    qDebug("finishedThread");

    //_audioInput->stop();
}

AudioInputDataThread::AudioInputDataThread(QAudioInput * audioInput, QIODevice * device, QVector<QByteArray> * byteVector)
{
    _audioInput = audioInput;
    _device     = device;
    //_byteArray  = byteArray;
    _byteVector = byteVector;
    _exitThread = false;
}

void AudioInputDataThread::run()
{
    QByteArray temp;
    QMutex mutex;
    while(!_exitThread)
    {
        mutex.lock();
        if( _audioInput->bytesReady() >= _audioInput->periodSize() )
        {
            temp = _device->read(_audioInput->periodSize());
            _device->seek(0);

            _byteVector->append(temp);

            //qDebug(qPrintable("Pos: " + QString::number(_device->pos())));
            //qDebug(qPrintable("Pos: " + QString::number(_device->)));

            /*if(_device->isOpen())
            {
                qDebug("open");
            }

            if(_device->isReadable())
            {
                qDebug("redable");
            }*/

            //_byteVector->append(_device->read(_audioInput->periodSize()));
            //_byteVector->append(_device->read(_audioInput->bytesReady()));
            //_byteArray->append( _device->read(_audioInput->bytesReady()) );
            //qDebug(qPrintable("BufferSize:    " + QString::number(_audioInput->bufferSize())));
            //qDebug(qPrintable("ByteArraySize: " + QString::number(_byteArray->size())));
            qDebug(qPrintable("Packets: " + QString::number(_byteVector->size())));
            //qDebug(qPrintable("Packet size: " + QString::number(_byteVector->last().size())));
        }
        mutex.unlock();
    }

    //qDebug(qPrintable("Bytes read: " + QString::number(_byteArray->size()))); // ####

    return;

    exec();
}
