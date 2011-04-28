#include "audio.h"
#include "QMutex"

Audio::Audio(QAudioFormat format, QAudioDeviceInfo device, QList<QByteArray> * byteList, QAudio::Mode mode)
{
    _format      = format;
    _device      = device;

    _audioInput  = NULL;
    _audioOutput = NULL;

    _byteList    = byteList;

    _mode        = mode;
}

void Audio::finishedAudio(QAudio::State state)
{
    if(state == QAudio::StoppedState)
    {
        QObject * sender = QObject::sender();
        sender->disconnect();

        delete sender;

        emit finished();
    }
}

void Audio::finishedThread()
{
    QObject * sender = QObject::sender();
    sender->disconnect();

    delete sender;
}

void Audio::initInput()
{
    stopInput();

    _audioInput   = new QAudioInput(_device, _format);
    QObject::connect(_audioInput, SIGNAL( stateChanged ( QAudio::State ) ), this, SLOT( finishedAudio( QAudio::State ) ) );
}

void Audio::initOutput()
{
    stopOutput();

    _audioOutput  = new QAudioOutput(_device, _format);
    QObject::connect(_audioOutput, SIGNAL( stateChanged ( QAudio::State ) ), this, SLOT( finishedAudio( QAudio::State ) ) );
}

void Audio::stopInput()
{
    if(_audioInput != NULL)
    {
        _audioInput->stop();
        _audioInput = NULL;
    }
}

void Audio::stopOutput()
{
    if(_audioOutput != NULL)
    {
        _audioOutput->stop();
        _audioOutput = NULL;
    }
}

QAudio::Mode Audio::mode()
{
    return _mode;
}
