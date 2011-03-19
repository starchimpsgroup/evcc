#include "audio.h"
#include "QMutex"

Audio::Audio(QAudioFormat format, QAudioDeviceInfo device, QVector<QByteArray> * byteVector, QAudio::Mode mode)
{
    _format      = format;
    _device      = device;

    _audioInput  = NULL;
    _audioOutput = NULL;

    /*if(mode == QAudio::AudioInput)
    {
        initInput();
    }
    else if(mode == QAudio::AudioOutput)
    {
        initOutput();
    }*/

    _byteVector  = byteVector;

}

void Audio::finishedAudio(QAudio::State state)
{
    if(state == QAudio::StoppedState)
    {
        QObject * sender = QObject::sender();
        sender->disconnect();

        delete sender;

        emit finished();

        qDebug("finishedAudio");
    }
}

void Audio::finishedThread()
{
    QObject * sender = QObject::sender();
    sender->disconnect();

    delete sender;

    qDebug("finishedThread");
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
