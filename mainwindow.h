#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAudioFormat>
#include "audioinput.h"
#include "audiooutput.h"
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

    QByteArray  * _audioInputByteArray;
    QByteArray  * _audioOutputByteArray;
    AudioInput  * _audioInput;
    AudioOutput * _audioOutput;

    QMap<Audio *, QByteArray *> _audioMap;

private slots:
    void on_call_clicked();
    void on_endCall_clicked();
    void finishedThread();

};

#endif // MAINWINDOW_H
