#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    _audioInput = NULL;

    // Set up the format, eg.
    _format.setFrequency(8000); // 44100
    _format.setChannels(1);
    _format.setSampleSize(16);
    _format.setCodec("audio/pcm");
    _format.setByteOrder(QAudioFormat::LittleEndian);
    _format.setSampleType(QAudioFormat::SignedInt);
}

MainWindow::~MainWindow()
{
    stopAudioInput();

    delete ui;
}

void MainWindow::on_call_clicked()
{
    stopAudioInput();

    _audioInputByteArray = new QByteArray();

    _audioInput = new AudioInput(_format, audioDeviceByName("pulse", QAudio::AudioInput), _audioInputByteArray);
    QObject::connect(_audioInput, SIGNAL( finished() ), this, SLOT( finishedThread() ) );

    _audioMap[_audioInput] = _audioInputByteArray;

    _audioInput->start();

    qDebug("record start");
}

void MainWindow::on_endCall_clicked()
{
    stopAudioInput();
}

void MainWindow::stopAudioInput()
{
    if(_audioInput != NULL && _audioInput->isRunning())
    {
        _audioInput->stop();
        _audioInput = NULL;

        qDebug("record stop");
    }
}

void MainWindow::finishedThread()
{
    Audio * sender = (Audio *)QObject::sender();
    sender->disconnect();

    delete _audioMap.take(sender);
    delete sender;
    qDebug("finished");
}

/**
  * ATTENTION: The default devices give a null-device at second call
  */
QAudioDeviceInfo MainWindow::audioDeviceByName( QString name, QAudio::Mode mode )
{
    QList<QAudioDeviceInfo> devices = QAudioDeviceInfo::availableDevices( mode );

    while(!devices.isEmpty())
    {
        QAudioDeviceInfo temp = devices.takeFirst();
        if(temp.deviceName() == name)
        {
            return temp;
        }
    }

    if( mode == QAudio::AudioInput)
    {
        return QAudioDeviceInfo::defaultInputDevice();
    }

    if( mode == QAudio::AudioOutput )
    {
        return QAudioDeviceInfo::defaultOutputDevice();
    }

    return QAudioDeviceInfo();
}
