#include "audiooutput.h"
//#include "QMutex"

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
    _audioOutput->reset();
    _stream = new QDataStream(_byteArray, QIODevice::ReadOnly /*| QIODevice::Unbuffered*/);

    _ioDevice = _stream->device();

     _ioDevice->open(QIODevice::ReadOnly);
    _audioOutput->start(_ioDevice);

    //#########
    _audioThread = new AudioOutputDataThread(_ioDevice, _byteArray);
    //QObject::connect( _audioThread, SIGNAL( finished() ), this, SLOT( finishedThread() ) );

    _audioThread->start();
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

        //_ioDevice->close();

        //delete _ioDevice;
        delete _stream;

        emit finished();
    }
}

AudioOutputDataThread::AudioOutputDataThread(QIODevice * device, QByteArray * byteArray)
{
    _byteArray  = byteArray;
    _device     = device;
    _exitThread = false;
}

void AudioOutputDataThread::run()
{
    //int pos = 0;
    //int poss;
    while(!_exitThread)
    {
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
