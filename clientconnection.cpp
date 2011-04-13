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
    quint32 blockSize;
    QDataStream in(_socket);
    QDataStream &out = *_user->outputDataStream();
    in.setVersion(QDataStream::Qt_4_0);

    //if (blockSize == 0) {
    if (_socket->bytesAvailable() < (int)sizeof(quint32))
        return;

    in >> blockSize;
    //}

    if (_socket->bytesAvailable() < blockSize)
        return;

    qint32 typ;
    in >> typ;

    switch((ServerConnectionTyps::ConnectionTyp)typ)
    {
    case ServerConnectionTyps::CONNECTIONACCEPTED:
         out << connectionTyp(ServerConnectionTyps::USERNAME) << _user->name();
        _user->send();
        break;

    case ServerConnectionTyps::USERNAMEACCEPTED:
        emit connectionEstablished();
        out << connectionTyp(ServerConnectionTyps::USERNAMES);
        _user->send();
        break;

    case ServerConnectionTyps::USERNAMEDENIED:
        emit message(tr("User with this name is already logged in"), ServerMessages::ERROR);
        break;

    case ServerConnectionTyps::USERNAMES:
        _users.clear();
        QString name, key;

        quint32 len;
        in >> len;

        for(int i = 0; i < len; i++)
        {
            in >> name >> key;
            _users.insert(name, key);
        }

        break;
    }
}

void ClientConnection::displayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        emit message( tr("The host was not found. "
                         "Please check the host name and port settings."),
                     ServerMessages::ERROR);
        break;
    case QAbstractSocket::ConnectionRefusedError:
        emit message( tr("The connection was refused by the peer. "
                         "Make sure the server is running, "
                         "and check that the host name and port "
                         "settings are correct."),
                     ServerMessages::ERROR);
        break;
    default:
        emit message( tr("The following error occurred: %1.")
                     .arg(_socket->errorString()),
                     ServerMessages::ERROR);
    }
}
