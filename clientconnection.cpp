#include "clientconnection.h"

ClientConnection::ClientConnection(QString server, quint16 port, QString userName, QObject *parent) :
    QObject(parent)
{
    _server = server;
    _port   = port;

    _socket = new QTcpSocket(this);
    _socket->connectToHost(_server, _port);

    _user = new User(_socket);
    _user->setName(userName);

    connect(_socket, SIGNAL(readyRead()), this, SLOT(read()));
    connect(_socket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(displayError(QAbstractSocket::SocketError)));
}

ClientConnection::~ClientConnection()
{
    delete _user;
    delete _socket;
}

void ClientConnection::read()
{
    quint16 blockSize = 0;
    QDataStream in(_socket);
    in.setVersion(QDataStream::Qt_4_0);

    if (blockSize == 0) {
        if (_socket->bytesAvailable() < (int)sizeof(quint16))
            return;

        in >> blockSize;
    }

    if (_socket->bytesAvailable() < blockSize)
        return;

    QString input;
    in >> input;
}

void ClientConnection::displayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        emit message( tr("The host was not found. "
                         "Please check the host name and port settings."),
                     ServerMessages::INFORMATION);
        break;
    case QAbstractSocket::ConnectionRefusedError:
        emit message( tr("The connection was refused by the peer. "
                         "Make sure the fortune server is running, "
                         "and check that the host name and port "
                         "settings are correct."),
                     ServerMessages::INFORMATION);
        break;
    default:
        emit message( tr("The following error occurred: %1.")
                     .arg(_socket->errorString()),
                     ServerMessages::INFORMATION);
    }
}
