#ifndef AUDIOOUTPUT_H
#define AUDIOOUTPUT_H

#include "audio.h"
#include "QThread"

//! Thread for reading the audiopart list.
/*!
    Tackes the parts from the audio list for playing.
 */
class AudioOutputDataThread : public QThread
{
public:

    //! Constructor AudioOutputDataThread.
    /*!
        \param audioOutput Audio output interface
        \param device Output device
        \param byteArray Array for output data
        \param byteList List of readed audio packets
     */
    AudioOutputDataThread(QAudioOutput * audioOutput, QIODevice * device, QByteArray * byteArray, QList<QByteArray> * byteList);

    //! Starts the thread activity.
    /*!
        Takes bytes from the input list and append it at the output array end.
     */
    void run();

    //! Stops thread.
    /*!
        Sets the exit variable.
     */
    void stop() { _exitThread = true; }

    //! Audio interface error.
    /*!
        Returns the error state of the audio output interface.
     */
    bool error(){ return _error; }

private:
    bool                  _exitThread;  //!< Exit variable
    bool                  _error;       //!< Error status
    QAudioOutput        * _audioOutput; //!< Audio output interface
    QIODevice           * _device;      //!< Audio output device
    QByteArray          * _byteArray;   //!< Array of output sound data
    QList<QByteArray>   * _byteList;    //!< Audio part list
};

//! The AudioOutput class.
/*!
    Plays audio data on the device.
 */
class AudioOutput : public Audio
{
public:

    //! Constructor AudioOutput class.
    /*!
        All parameters for base class
        \param format Format parameter of the audio output
        \param device Chosen device for audio output
        \param byteList List of audio parts
     */
    AudioOutput(QAudioFormat format, QAudioDeviceInfo device, QList<QByteArray> * byteList);

    //! Destructor AudioOutput class.
    ~AudioOutput();

    //! Start audio output.
    /*!
        Initialize audio output and starts AudioOutputDataThread.
    */
    void start();

    //! Stop audio output.
    /*!
        Stops AudioOutputDataThread.
    */
    void stop();

    //! Audio data stream.
    /*!
        Returned for delete.
        \return Audio data stream
    */
    QDataStream * stream(){ return _stream; }

    //! Audio data array.
    /*!
        Returned for delete.
        \return Audio byte array
    */
    QByteArray  * byteArray(){ return _byteArray; }

private:

    //! Initialize audio output.
    /*!
        Initialize audio output and starts AudioOutputDataThread.
    */
    void init();

private:
    AudioOutputDataThread * _audioThread; //!< Audio data thread
    QDataStream           * _stream;      //!< Audio data stream
    QIODevice             * _ioDevice;    //!< Audio output device
    QByteArray            * _byteArray;   //!< Audio data array

protected slots:
    //! Called by statechange of audio interface.
    /*!
        Disconnect and delete interface by stopped state.
        If an error ocurred the audio output will be restarted.
     */
    virtual void finishedThread();
    void finishedAudio( QAudio::State state );
};

#endif // AUDIOOUTPUT_H
