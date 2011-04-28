#ifndef PLAYFILE_H
#define PLAYFILE_H

#include <QObject>
#include <QAudioOutput>
#include <QFile>

//! The PlayFile class.
/*!
    Plays wave audio files.
 */
class PlayFile : public QObject
{
    Q_OBJECT
public:
    //! Constructor PlayFile class.
    /*!
        Set audio format, reads audio file and initialise audio output
        \param file File to open
        \param loop Value for loop
        \param parent Parent widget
     */
    explicit PlayFile(QString file, bool loop = true, QObject *parent = 0);

    //! Destructor PlayFile class.
    ~PlayFile();

    //! Play sound.
    /*!
        Open device and start audio output
     */
    void play();

    //! Stop sound.
    /*!
        Close device and stop audio output
     */
    void stop();

private:
    QFile          _inputFile; //!< Input sound file
    QAudioOutput * _audio;     //!< Audio output interface
    QAudioFormat   _format;    //!< Format of audio file
    QByteArray     _file;      //!< Readed audio file
    QDataStream  * _stream;    //!< Stream for audio output
    QIODevice    * _ioDevice;  //!< Stream device
    bool           _loop;      //!< Loop falue

private slots:
    //! Finished playing.
    /*!
        Stops timer on idle state ore play sound again if loop falue is true
        \see playAgain
        \param state State of audio device
     */
    void finishedPlaying(QAudio::State state);

    //! Play Again.
    /*!
        Reset device and start playind
     */
    void playAgain();

};

#endif // PLAYFILE_H
