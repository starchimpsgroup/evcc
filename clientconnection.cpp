#include "clientconnection.h"

ClientConnection::ClientConnection(QString server, quint16 port, QString userName, QObject *parent) :
    QObject(parent)
{
    _state  = ClientConnection::IDLE;

    _server = server;
    _port   = port;

    _socket = new QTcpSocket(this);
    _socket->connectToHost(_server, _port);

    _user = new User(_socket);
    _user->setName(userName);

    QObject::connect(_socket, SIGNAL(readyRead()), this, SLOT(read()));
    QObject::connect(_socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    QObject::connect(_socket, SIGNAL(error(QAbstractSocket::SocketError)),
                        this, SLOT(displayError(QAbstractSocket::SocketError)));
}

ClientConnection::~ClientConnection()
{
    delete _user;
}

void ClientConnection::disconnected()
{
    _socket->blockSignals(true);
    emit message(tr("Disconnected from server"), ServerMessages::ERROR);
}

void ClientConnection::call(QString name)
{
    _state  = ClientConnection::CALLING;
    _userCalling = name;
    *_user->outputDataStream() << connectionTyp(ServerConnectionTyps::CALL) << name;
    _user->send();
}

void ClientConnection::callEnd()
{
    *_user->outputDataStream() << connectionTyp(ServerConnectionTyps::CALLEND);
    _user->send();
}

void ClientConnection::read()
{
    quint32 blockSize;
    QDataStream in(_socket);
    QDataStream &out = *_user->outputDataStream();
    in.setVersion(QDataStream::Qt_4_0);

    if (_socket->bytesAvailable() < (int)sizeof(qint32))
        return;

    in >> blockSize;

    if (_socket->bytesAvailable() < blockSize)
        return;

    qint32 typ;
    in >> typ;

    switch((ServerConnectionTyps::ConnectionTyp)typ)
    {
    case ServerConnectionTyps::CONNECTIONACCEPTED:
    {
        out << connectionTyp(ServerConnectionTyps::USERNAME) << _user->name() << _user->publicKey();
        _user->send();
        break;
    }
    case ServerConnectionTyps::USERNAMEACCEPTED:
    {
        emit connectionEstablished();
        break;
    }
    case ServerConnectionTyps::USERNAMEDENIED:
    {
        //_error = true;
        emit message(tr("User with this name is already logged in"), ServerMessages::ERROR);
        break;
    }
    case ServerConnectionTyps::USERNAMES:
    {
        _users.clear();
        QString name, key;

        quint32 len;
        in >> len;

        for(uint i = 0; i < len; i++)
        {
            in >> name >> key;

            if(name != _user->name())
                _users.insert(name, key);
        }

        emit userListRefresh();

        break;
    }
    case ServerConnectionTyps::CALLEND:
    {
        // if partner/server terminates
        emit callTerminated();
        break;
    }
    case ServerConnectionTyps::CALLACCEPTED:
    {
        // wait for calletablished
        emit callOut(_userCalling);
        break;
    }
    case ServerConnectionTyps::CALLDENIED:
    {
        // if partner is calling
        emit callDenied(_userCalling);
        break;
    }
    case ServerConnectionTyps::CALL:
    {
        // callacept / denied
        QString name;
        in >> name;
        emit callIn(name);
        break;
    }
    default:
        //emit message(tr("Undefined connection typ"), ServerMessages::WARNING);
        return;
    }

    if(!in.atEnd())
    {
        read();
    }
}

void ClientConnection::displayError(QAbstractSocket::SocketError socketError)
{
    //_error = true;

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
