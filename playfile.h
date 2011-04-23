#ifndef PLAYFILE_H
#define PLAYFILE_H

#include <QObject>
#include <QAudioOutput>
#include <QFile>

class PlayFile : public QObject
{
    Q_OBJECT
public:
    explicit PlayFile(QString file, bool loop = true, QObject *parent = 0);
    ~PlayFile();
    void play();
    void stop();

private:
    QFile          _inputFile;
    QAudioOutput * _audio;
    QAudioFormat   _format;
    QByteArray     _file;
    QDataStream  * _stream;
    QIODevice    * _ioDevice;
    bool           _loop;

signals:

private slots:
    void finishedPlaying(QAudio::State state);
    void playAgain();

};

#endif // PLAYFILE_H
