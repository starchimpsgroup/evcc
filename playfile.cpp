#include "playfile.h"
#include <QTimer>

PlayFile::PlayFile(QString file, bool loop, QObject *parent) :
    QObject(parent), _inputFile(file), _loop(loop)
{
    _format.setFrequency(8000);
    _format.setChannels(1);
    _format.setSampleSize(8);
    _format.setCodec("audio/pcm");
    _format.setByteOrder(QAudioFormat::LittleEndian);
    _format.setSampleType(QAudioFormat::UnSignedInt);

    _inputFile.open(QIODevice::ReadOnly);
    _file  = _inputFile.readAll();
    _inputFile.close();

    _file  = _file.right(_file.size()-44);

    _audio = new QAudioOutput(_format, this);
    QObject::connect(_audio, SIGNAL(stateChanged(QAudio::State)), this, SLOT(finishedPlaying(QAudio::State)));

    _stream   = new QDataStream(&_file, QIODevice::ReadOnly);
    _ioDevice = _stream->device();
}

PlayFile::~PlayFile()
{
    _audio->stop();
    _ioDevice->close();
    delete _audio;
    delete _stream;
}

void PlayFile::play()
{
    _ioDevice->open(QIODevice::ReadOnly);
    _audio->start(_ioDevice);
}

void PlayFile::stop()
{
    _audio->stop();
    _ioDevice->close();
}

void PlayFile::finishedPlaying(QAudio::State state)
{
    if(state == QAudio::IdleState)
    {
        if(_loop)
        {
            QTimer::singleShot(1500, this, SLOT(playAgain()));
        }
        else
        {
            stop();
        }
    }
}

void PlayFile::playAgain()
{
    if(_ioDevice->isOpen())
    {
        _ioDevice->seek(0);
        _audio->start(_ioDevice);
    }
}
