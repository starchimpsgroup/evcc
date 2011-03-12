#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAudioFormat>
#include "audioinput.h"
#include "audiooutput.h"
#include "spectrum.h"
#include <QMap>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void stopAudioInput();
    void startAudioInput();

    void stopAudioOutput();
    void startAudioOutput();

    QAudioDeviceInfo audioDeviceByName( QString name, QAudio::Mode mode );

private:
    Ui::MainWindow * ui;

    QAudioFormat _format;

    //QByteArray  * _audioInputByteArray;
    //QByteArray  * _audioOutputByteArray;
    QVector<QByteArray> * _audioInputVector;
    QVector<QByteArray> * _audioOutputVector;
    AudioInput  * _audioInput;
    AudioOutput * _audioOutput;

    //QMap<QObject *, QByteArray *> _audioMap;

    Spectrum   * _spectrum;

private slots:
    void on_call_clicked();
    void on_endCall_clicked();
    //void finishedAudio();

};

#endif // MAINWINDOW_H
