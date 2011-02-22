#ifndef AUDIOOUTPUT_H
#define AUDIOOUTPUT_H

#include "audio.h"

class AudioOutput : public Audio
{
public:
    AudioOutput(QAudioFormat format, QAudioDeviceInfo device, QByteArray * byteArray);

    void run();
};

#endif // AUDIOOUTPUT_H
