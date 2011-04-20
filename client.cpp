#include "client.h"
#include "ui_client.h"
#include <QMessageBox>

Client::Client(QString server, quint16 port, QString userName, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Client)
{
    ui->setupUi(this);

    ui->userList->setStyleSheet("background-color: white;"
                                "background-image: url(:/logos/clientLogo.png);"
                                "background-repeat: none;"
                                "background-position: center;");

    _server = server;
    _port   = port;

    _connection = new ClientConnection(server, port, userName, this);

    QObject::connect(_connection,
                     SIGNAL(message(QString,ServerMessages::MessageTyp)),
                     this,
                     SLOT(showMessage(QString,ServerMessages::MessageTyp)));

    QObject::connect(_connection, SIGNAL(connectionEstablished()), this->parent(), SLOT(connectionEstablished()));
    QObject::connect(_connection, SIGNAL(connectionLost()),        this->parent(), SLOT(connectionLost()));
    QObject::connect(_connection, SIGNAL(userListRefresh()),       this, SLOT(userListRefresh()));
    QObject::connect(_connection, SIGNAL(callOut(QString)),        this->parent(), SLOT(callOut(QString)));
    QObject::connect(_connection, SIGNAL(callIn(QString)),         this->parent(), SLOT(callIn(QString)));
    QObject::connect(_connection, SIGNAL(callDenied(QString)),     this->parent(), SLOT(callDenied(QString)));
    QObject::connect(_connection, SIGNAL(callTerminated()),        this->parent(), SLOT(callTerminated()));

    // Set up the format, eg.
    _format.setFrequency(8000); // 44100 8000
    _format.setChannels(1);
    _format.setSampleSize(8); // 16
    _format.setCodec("audio/pcm");
    _format.setByteOrder(QAudioFormat::LittleEndian);
    _format.setSampleType(QAudioFormat::UnSignedInt); // SignedInt

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
    delete _connection;

    delete ui;
}

void Client::on_call_clicked()
{
    if(ui->userList->currentItem())
        _connection->call(ui->userList->currentItem()->text());
    /*stopAudioInput();
    startAudioInput();

    stopAudioOutput();
    startAudioOutput();*/
}

void Client::on_endCall_clicked()
{
    _connection->callEnd();
    /*stopAudioInput();
    stopAudioOutput();*/
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
    else if( mode == QAudio::AudioOutput )
    {
        return QAudioDeviceInfo::defaultOutputDevice();
    }

    return QAudioDeviceInfo();
}

void Client::showMessage(QString text, ServerMessages::MessageTyp typ)
{
    switch(typ)
    {
    case ServerMessages::INFORMATION:
    {
        QMessageBox::information(this, tr("Information"),
                                 text);
        break;
    }
    case ServerMessages::ERROR:
    {
        QMessageBox::critical(this, tr("Error"),
                                 text);
        emit serverError();
        break;
    }
    case ServerMessages::WARNING:
    {
        QMessageBox::warning(this, tr("Warning"),
                                 text);
        break;
    }
    default:
    {
    }
    }
}

void Client::userListRefresh()
{
    QList<QString> users = _connection->users();
    ui->userList->clear();
    while (!users.isEmpty())
        ui->userList->addItem( users.takeFirst() );
}
