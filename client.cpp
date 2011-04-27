#include "client.h"
#include "ui_client.h"
#include <QMessageBox>
#include <QMutex>

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

    _callEstablished = false;

    QObject::connect(_connection,
                     SIGNAL(message(QString,ServerMessages::MessageTyp)),
                     this,
                     SLOT(showMessage(QString,ServerMessages::MessageTyp)));

    QObject::connect(_connection, SIGNAL(connectionEstablished()), this->parent(), SLOT(connectionEstablished()));
    QObject::connect(_connection, SIGNAL(userListRefresh()),       this,           SLOT(userListRefresh()));
    QObject::connect(_connection, SIGNAL(callOut(QString)),        this->parent(), SLOT(callOut(QString)));
    QObject::connect(_connection, SIGNAL(callIn(QString)),         this->parent(), SLOT(callIn(QString)));
    QObject::connect(_connection, SIGNAL(callDenied(QString)),     this->parent(), SLOT(callDenied(QString)));
    QObject::connect(_connection, SIGNAL(callTerminated()),        this->parent(), SLOT(callTerminated()));
    QObject::connect(_connection, SIGNAL(callEstablished()),       this,           SLOT(callEstablished()));
    QObject::connect(_connection, SIGNAL(callTerminated()),        this,           SLOT(callTerminated()));
    QObject::connect(_connection, SIGNAL(callEstablished()),       this->parent(), SLOT(callEstablished()));
    QObject::connect(_connection, SIGNAL(receivedSoundData(QByteArray)), this,     SLOT(receivedSoundData(QByteArray)));
    QObject::connect(_connection, SIGNAL(dataTransferred()),       this,           SLOT(dataTransferred()));

    // Set up the format, eg.
    _format.setFrequency(8000); // 44100 8000
    _format.setChannels(1);
    //_format.setSampleSize(8); // 16
    _format.setSampleSize(16);
    _format.setCodec("audio/pcm");
    _format.setByteOrder(QAudioFormat::LittleEndian);
    //_format.setSampleType(QAudioFormat::UnSignedInt); // SignedInt
    _format.setSampleType(QAudioFormat::SignedInt);

    _audioInputList  = new QList<QByteArray>();
    _audioOutputList = new QList<QByteArray>();

    _audioInput  = new AudioInput ( _format, audioDeviceByName("pulse", QAudio::AudioInput  ), _audioInputList);
    _audioOutput = new AudioOutput( _format, audioDeviceByName("pulse", QAudio::AudioOutput ), _audioOutputList);
}

Client::~Client()
{
    callTerminated();

    delete _audioInputList;
    delete _audioOutputList;
    delete _audioInput;
    delete _audioOutput;
    delete _connection;

    delete ui;
}

void Client::on_call_clicked()
{
    if(_connection->state() == ClientConnection::IDLE)
    {
        if(ui->userList->currentItem())
            _connection->call(ui->userList->currentItem()->text());
    }
    else if(_connection->state() == ClientConnection::INCOMINGCALL)
    {
        _connection->callAccept();
    }
}

void Client::on_endCall_clicked()
{
    if(_connection->state() == ClientConnection::INCOMINGCALL || _connection->state() == ClientConnection::CALLING)
    {
        _connection->callEnd();
    }
}

void Client::callEstablished()
{
    _callEstablished = true;

    stopAudioInput();
    startAudioInput();

    stopAudioOutput();
    startAudioOutput();

    _audioInputList->clear();

    _dataThread = new OutputDataThread(_connection, _audioInputList);
    QObject::connect( _dataThread, SIGNAL( finished() ), this, SLOT( finishedThread() ) );

    _dataThread->sendData();

    _dataThread->start();
}

void Client::receivedSoundData(QByteArray data)
{
    _audioOutputList->append(data);
}

void Client::dataTransferred()
{
    _dataThread->sendData();
}

void Client::callTerminated()
{
    if(_callEstablished)
    {
        _callEstablished = false;

        stopAudioInput();
        stopAudioOutput();

        _dataThread->stop();
    }
}

void Client::stopAudioInput()
{
    _audioInput->stop();
    _audioInputList->clear();

    qDebug("record stop");
}

void Client::startAudioInput()
{
    _audioInputList->clear();
    _audioInput->start();

    qDebug("record start");
}

void Client::stopAudioOutput()
{
    _audioOutput->stop();
    _audioOutputList->clear();

    qDebug("play stop");
}

void Client::startAudioOutput()
{
    _audioOutputList->clear();
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
        callTerminated();
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

void Client::finishedThread()
{
    QObject * sender = QObject::sender();
    sender->disconnect();

    delete sender;
}

OutputDataThread::OutputDataThread(ClientConnection * connection, QList<QByteArray> * byteList)
{
    _byteList   = byteList;
    _connection = connection;
    _exitThread = false;
    _sendData   = 0;
}

void OutputDataThread::run()
{
    _byteList->clear();

    QMutex mutex;
    while(!_exitThread)
    {
        mutex.lock();
        if(_sendData > 0 && !_byteList->isEmpty())
        {
            _sendData--;
            _connection->sendAudioData(_byteList->takeFirst());
        }
        mutex.unlock();
    }

    return;

    exec();
}
