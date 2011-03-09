#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    _audioInput  = NULL;
    _audioOutput = NULL;

    // Set up the format, eg.
    _format.setFrequency(8000); // 44100 8000
    _format.setChannels(1);
    _format.setSampleSize(16);
    _format.setCodec("audio/pcm");
    _format.setByteOrder(QAudioFormat::LittleEndian);
    _format.setSampleType(QAudioFormat::SignedInt);

    // ##################
/*
    QList<QAudioDeviceInfo> i = QAudioDeviceInfo::availableDevices(QAudio::AudioInput);
    QList<QAudioDeviceInfo> o = QAudioDeviceInfo::availableDevices(QAudio::AudioOutput);

    QAudioDeviceInfo outputDevice = i.at(0);//QAudioDeviceInfo::defaultOutputDevice();
    QAudioDeviceInfo inputDevice  = i.at(1);//QAudioDeviceInfo::defaultInputDevice();

    QAudioInput    * _audioInput;
    QAudioOutput   * _audioOutput;

    _audioOutput = new QAudioOutput(outputDevice, _format);
    _audioInput  = new QAudioInput(inputDevice, _format);

    QIODevice * ioDevice;
    ioDevice = _audioInput->start();
    _audioOutput->start(ioDevice);
*/
    // ##################
}

MainWindow::~MainWindow()
{
    stopAudioInput();
    stopAudioOutput();

    delete ui;
}

void MainWindow::on_call_clicked()
{
    stopAudioInput();
    startAudioInput();

    stopAudioOutput();
    startAudioOutput();
}

void MainWindow::on_endCall_clicked()
{
    stopAudioInput();
    stopAudioOutput();
}

void MainWindow::stopAudioInput()
{
    if(_audioInput != NULL)
    {
        _audioInput->stop();
        _audioInput = NULL;

        qDebug("record stop");
    }
}

void MainWindow::startAudioInput()
{
    _audioInputByteArray = new QByteArray();

    _audioInput = new AudioInput(_format, audioDeviceByName("pulse", QAudio::AudioInput), _audioInputByteArray);
    QObject::connect(_audioInput, SIGNAL( finished() ), this, SLOT( finishedAudio() ) );

    _audioMap[_audioInput] = _audioInputByteArray;
qDebug(qPrintable("new_byteArrayInput_Adress  " + QString::number((int)_audioInputByteArray))); // ###
qDebug(qPrintable("new_audioInput_Adress      " + QString::number((int)_audioInput))); // ###
    _audioInput->start();

    qDebug("record start");
}

void MainWindow::stopAudioOutput()
{
    if(_audioOutput != NULL)
    {
        _audioOutput->stop();
        _audioOutput = NULL;

        qDebug("play stop");
    }
}

void MainWindow::startAudioOutput()
{
    _audioOutputByteArray = new QByteArray();

    //_audioOutputByteArray = _audioInputByteArray;

    _audioOutput = new AudioOutput(_format, audioDeviceByName("pulse", QAudio::AudioInput), _audioOutputByteArray);
    QObject::connect(_audioOutput, SIGNAL( finished() ), this, SLOT( finishedAudio() ) );

    //_audioMap[_audioOutput] = _audioOutputByteArray;
qDebug(qPrintable("new_byteArrayOutput_Adress " + QString::number((int)_audioOutputByteArray))); // ###
qDebug(qPrintable("new_audioOutput_Adress     " + QString::number((int)_audioOutput))); // ###
    _audioOutput->start();

    qDebug("play start");
}

void MainWindow::finishedAudio()
{
        QObject * sender = QObject::sender();
        sender->disconnect();
 qDebug(qPrintable("delete_byteArray_Adress " + QString::number((int)_audioMap[sender]))); // ###
 qDebug(qPrintable("delete_audio_Adress     " + QString::number((int)sender))); // ###
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
