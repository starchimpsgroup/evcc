#include "client.h"
#include "ui_client.h"
#include <QMessageBox>

Client::Client(QString server, quint16 port, QString userName, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Client)
{
    ui->setupUi(this);

    _server = server;
    _port   = port;

    /*_connection = new ClientConnection(server, port, userName);

    connect(_connection,
            SIGNAL(message(QString,ServerMessages::MessageTyp)),
            this,
            SLOT(showMessage(QString,ServerMessages::MessageTyp)));*/

    // Set up the format, eg.
    _format.setFrequency(8000); // 44100 8000
    _format.setChannels(1);
    _format.setSampleSize(16);
    _format.setCodec("audio/pcm");
    _format.setByteOrder(QAudioFormat::LittleEndian);
    _format.setSampleType(QAudioFormat::SignedInt);

    _audioInputVector  = new QVector<QByteArray>();
    _audioOutputVector = new QVector<QByteArray>();

    _audioInput  = new AudioInput ( _format, audioDeviceByName("pulse", QAudio::AudioInput  ), _audioInputVector);
    _audioOutput = new AudioOutput( _format, audioDeviceByName("pulse", QAudio::AudioOutput ), _audioInputVector);
}

Client::~Client()
{
    stopAudioInput();
    stopAudioOutput();

    delete _audioInputVector;
    delete _audioOutputVector;
    delete _audioInput;
    delete _audioOutput;
    //delete _connection;

    delete ui;
}

void Client::on_call_clicked()
{
    stopAudioInput();
    startAudioInput();

    stopAudioOutput();
    startAudioOutput();
}

void Client::on_endCall_clicked()
{
    stopAudioInput();
    stopAudioOutput();
}

void Client::stopAudioInput()
{
    _audioInput->stop();
    _audioInputVector->clear();

    qDebug("record stop");
}

void Client::startAudioInput()
{
    _audioInputVector->clear();
    _audioInput->start();

    qDebug("record start");
}

void Client::stopAudioOutput()
{
    _audioOutput->stop();
    _audioOutputVector->clear();

    qDebug("play stop");
}

void Client::startAudioOutput()
{
    _audioOutputVector->clear();
    _audioOutput->start();

    qDebug("play start");
}

/**
  * ATTENTION: The default devices give a null-device at second call
  */
QAudioDeviceInfo Client::audioDeviceByName( QString name, QAudio::Mode mode )
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

void Client::showMessage(QString text, ServerMessages::MessageTyp typ)
{
    if(typ == ServerMessages::INFORMATION)
    {
        QMessageBox::information(this, tr("Information"),
                                 text);
    }
}
