#ifndef AUDIOINPUT_H
#define AUDIOINPUT_H

#include "audio.h"

class AudioInput : public Audio
{
public:
    AudioInput(QAudioFormat format, QAudioDeviceInfo device, QByteArray * byteArray);

    void run();
};

#endif // AUDIOINPUT_H
