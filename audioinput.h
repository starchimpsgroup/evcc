#ifndef AUDIOINPUT_H
#define AUDIOINPUT_H

#include "audio.h"
#include <QThread>

//! Thread for filling the audiopart list.
/*!
    Fills the list with same sized packets from the audio device.
 */
class AudioInputDataThread : public QThread
{
public:

    //! Constructor AudioInputDataThread.
    /*!
        \param audioInput Audio input interface
        \param device Input device
        \param byteList List of readed audio packets
     */
    AudioInputDataThread(QAudioInput * audioInput, QIODevice * device, QList<QByteArray> * byteList);

    //! Starts the thread activity.
    /*!
        Takes bytes from the device, of the periodsize from the interface, and append it at the list end.
     */
    void run();

    //! Stops thread.
    /*!
        Sets the exit variable.
     */
    void stop(){ _exitThread = true; }

private:
    bool                _exitThread; //!< Exit variable
    QAudioInput       * _audioInput; //!< Audio input interface
    QIODevice         * _device;     //!< Audio input device
    QList<QByteArray> * _byteList;   //!< Audio part list

};

//! The AudioInput class.
/*!
    Reads audio data form the device.
 */
class AudioInput : public Audio
{
public:

    //! Constructor AudioInput class.
    /*!
        All parameters for base class
        \param format Format parameter of the audio input
        \param device Chosen device for audio input
        \param byteList List of audio parts
     */
    AudioInput(QAudioFormat format, QAudioDeviceInfo device, QList<QByteArray> * byteList);

    //! Destructor AudioInput class.
    ~AudioInput();

    //! Start audio input.
    /*!
        Initialize audio input and starts AudioInputDataThread.
    */
    void start();

    //! Stop audio input.
    /*!
        Stops AudioInputDataThread.
    */
    void stop();

private:
    AudioInputDataThread * _audioThread; //!< Audio data thread
    QIODevice            * _ioDevice;    //!< Audio input device

protected slots:
    void finishedThread();
};

#endif // AUDIOINPUT_H
