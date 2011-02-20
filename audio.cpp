#include "audio.h"

Audio::Audio(QAudioFormat format, QAudioDeviceInfo device, QByteArray * byteArray, QAudio::Mode mode)
{
    _format      = format;
    _device      = device;

    if(mode == QAudio::AudioInput)
    {
        _audioInput   = new QAudioInput(_device, _format);
    }

    if(mode == QAudio::AudioOutput)
    {
        _audioOutput  = new QAudioOutput(_device, _format);
    }

    _byteArray  = byteArray;
    _exitThread = false;
}
