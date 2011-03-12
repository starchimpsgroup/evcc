#include "audiooutput.h"
#include "QMutex"

AudioOutput::AudioOutput(QAudioFormat format, QAudioDeviceInfo device, QVector<QByteArray> * byteVector) :
                   Audio(format, device, byteVector, QAudio::AudioOutput)
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

    _byteVector->clear();
    initOutput();
    _byteArray = new QByteArray();
    _stream = new QDataStream(_byteArray, QIODevice::ReadOnly /*| QIODevice::Unbuffered*/);

    _ioDevice = _stream->device();

    _ioDevice->open(QIODevice::ReadOnly);
    _audioOutput->start(_ioDevice);

    _audioThread = new AudioOutputDataThread(_ioDevice, _byteArray, _byteVector);
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
        QObject * sender = QObject::sender();
        sender->disconnect();

        delete sender;

        //_ioDevice->close();

        //delete _ioDevice;
        delete _stream;

        emit finished();

        qDebug("finishedAudio");
    }
}

AudioOutputDataThread::AudioOutputDataThread(QIODevice * device, QByteArray * byteArray, QVector<QByteArray> * byteVector)
{
    _byteArray  = byteArray;
    _byteVector = byteVector;
    _device     = device;
    _exitThread = false;
}

void AudioOutputDataThread::run()
{
    //int pos = 0;
    //int poss;
    QMutex mutex;
    while(!_exitThread)
    {
        mutex.lock();
        if(_device->atEnd() && !_byteVector->isEmpty())
        {
            *_byteArray = _byteVector->first();
            _byteVector->pop_front();
            _device->reset();

            qDebug("output");
        }
        mutex.unlock();

        /*QMutex mutex;
        mutex.lock();
        poss = _byteArray->size();
        if(poss > pos)
        {
            qDebug(qPrintable(QString::number(_byteArray->at(pos))));
            pos++;
        }
        mutex.unlock();*/

        /*poss = _device->pos();
        if(pos != poss)
        {
            pos = poss;
            qDebug(qPrintable("Position: " + QString::number(poss)));
        }*/
    }

    //qDebug(qPrintable("Bytes read: " + QString::number(_byteArray->size()))); // ####

    return;

    exec();
}
