#include "serverconnection.h"
#include <QMessageBox>

ServerConnection::ServerConnection(quint16 port, QHostAddress host, QObject* parent) :
    QTcpServer(parent), _hostAddress(host), _port(port)
{
    if ( !listen( _hostAddress, _port ) )
    {
        _serverStatus    = tr("Unable to start the server: %1.")
                           .arg(this->errorString());
        _serverStatusTyp = ServerMessages::ERRORMESSAGE;

        return;
    }

    _serverStatus    = tr("Server started.");
    _serverStatusTyp = ServerMessages::TEXT;

    QCA::init();
};

User * ServerConnection::getUserByName(QString name)
{
    if(!_users.isEmpty())
    {
        QHashIterator<QTcpSocket*, User*> i(_users);
        while (i.hasNext())
        {
            i.next();

            if(i.value()->name() == name)
                return i.value();
        }
    }

    return NULL;
}

bool ServerConnection::isNameExistent(QString name)
{
    return _usernames.contains(name);
}

void ServerConnection::incomingConnection ( int socketDescriptor )
{
    QTcpSocket * incomingSocket = new QTcpSocket(this);
    incomingSocket->setSocketDescriptor(socketDescriptor);
    addPendingConnection(incomingSocket);

    emit message(tr("Connect: ") + incomingSocket->peerAddress().toString() + ":"
                                 + QString::number(incomingSocket->peerPort()), ServerMessages::SEND);

    QObject::connect(incomingSocket, SIGNAL(disconnected()), this, SLOT(socketDisconnected()));
    QObject::connect(incomingSocket, SIGNAL(readyRead()),    this, SLOT(socketReadyRead()));

    _users.insert(incomingSocket, new User(incomingSocket));
    User * u = _users[incomingSocket];
    *u->outputDataStream() << connectionTyp(ServerConnectionTyps::CONNECTIONACCEPTED);
    u->send();
}

void ServerConnection::usernames()
{
    if(!_users.isEmpty())
    {
        QHashIterator<QTcpSocket*, User*> u(_users);

        while (u.hasNext())
        {
            u.next();
            QDataStream &out = *u.value()->outputDataStream();
            out << connectionTyp(ServerConnectionTyps::USERNAMES) << (quint32)_users.size();

            QHashIterator<QTcpSocket*, User*> i(_users);
            while (i.hasNext())
            {
                i.next();

                out << i.value()->name() << i.value()->publicKey().toDER();
            }

            u.value()->send();
        }
    }
}

void ServerConnection::socketReadyRead()
{
    QTcpSocket * socket = (QTcpSocket *)QObject::sender();
    User * u = _users[socket];

    QDataStream in(socket);
    QDataStream &out = *u->outputDataStream();
    in.setVersion(QDataStream::Qt_4_0);

    qDebug("bytesAvailable: %li", (long)socket->bytesAvailable());

    if (u->blockSize() == 0) {
        if (socket->bytesAvailable() < (int)sizeof(qint32))
            return;

        in >> u->blockSize();
        qDebug("blockSize: %i", u->blockSize());
    }

    if (socket->bytesAvailable() < u->blockSize())
        return;

    u->blockSize() = 0;

    qint32 typ;
    in >> typ;

    qDebug("typ: %i", typ);

    switch((ServerConnectionTyps::ConnectionTyp)typ)
    {
    case ServerConnectionTyps::USERNAMES:
    {
        out << connectionTyp(ServerConnectionTyps::USERNAMES) << (quint32)_users.size();

        if(!_users.isEmpty())
        {
            QHashIterator<QTcpSocket*, User*> i(_users);
            while (i.hasNext())
            {
                i.next();

                out << i.value()->name() << i.value()->publicKey().toDER();
            }
        }
        u->send();
        break;
    }
    case ServerConnectionTyps::USERNAME:
    {
        QString name;
        QByteArray key;
        in >> name >> key;

        if(isNameExistent(name))
        {
            out << connectionTyp(ServerConnectionTyps::USERNAMEDENIED);
            u->send();
        }
        else
        {
            out << connectionTyp(ServerConnectionTyps::USERNAMEACCEPTED);
            u->send();

            u->setData(name, key);
            _usernames.append(name);
            usernames();
        }
        break;
    }
    case ServerConnectionTyps::CALL:
    {
        QString name;

        in >> name;

        if(isNameExistent(name) && !getUserByName(name)->isCalling())
        {
            out << connectionTyp(ServerConnectionTyps::CALLACCEPTED);
            getUserByName(name)->setCalling( u );
            u->setCalling( getUserByName(name) );

            *getUserByName(name)->outputDataStream() << connectionTyp(ServerConnectionTyps::CALL) << u->name();
            getUserByName(name)->send();

            emit message(tr("CallAccepted: ") + u->name() + " >> "
                         + name, ServerMessages::TEXT);
        }
        else
        {
            out << connectionTyp(ServerConnectionTyps::CALLDENIED);
            emit message(tr("CallDenied: ") + u->name() + " >> "
                         + name, ServerMessages::TEXT);
        }

        u->send();
        break;
    }
    case ServerConnectionTyps::CALLEND:
    {
        callEnd(u);
        break;
    }
    case ServerConnectionTyps::CALLACCEPTED:
    {
        if(u->isCalling())
        {
            *u->partner()->outputDataStream() << connectionTyp(ServerConnectionTyps::CALLESTABLISHED);
            u->partner()->send();

            out << connectionTyp(ServerConnectionTyps::CALLESTABLISHED);
            u->send();
        }

        break;
    }
    case ServerConnectionTyps::AUDIODATA:
    {
        if(u->isCalling())
        {
            QByteArray data;
            in >> data;

            *u->partner()->outputDataStream() << connectionTyp(ServerConnectionTyps::AUDIODATA) << data;
            u->partner()->send();
            qDebug("in");
            out << connectionTyp(ServerConnectionTyps::AUDIODATATRANSFERRED);
            u->send();
        }
        else
        {
            _users.key(u->partner())->readAll();
            u->partner()->blockSize() = 0;

            _users.key(u)->readAll();
            u->blockSize() = 0;
        }
        break;
    }
    case ServerConnectionTyps::AUDIODATASIZE:
    {
        if(u->isCalling())
        {
            qint32 size;
            in >> size;

            *u->partner()->outputDataStream() << connectionTyp(ServerConnectionTyps::AUDIODATASIZE) << size;
            u->partner()->send();
        }
        break;
    }
    default:
        emit message(tr("Undefined connection typ"), ServerMessages::WARNING);
        return;
    }
}

void ServerConnection::callEnd(User * u)
{
    if(u->isCalling())
    {
        u->partner()->endCalling();
        u->endCalling();

        *u->partner()->outputDataStream() << connectionTyp(ServerConnectionTyps::CALLEND);
        u->partner()->send();

        *u->outputDataStream() << connectionTyp(ServerConnectionTyps::CALLEND);
        u->send();
    }
}

void ServerConnection::socketDisconnected()
{
    QTcpSocket * socket = (QTcpSocket *)QObject::sender();
    User * u =  _users.take(socket);

    emit message(tr("Diconnected: ") + socket->peerAddress().toString() + ":"
                                     + QString::number(socket->peerPort()), ServerMessages::SEND);

    callEnd(u);

    _usernames.removeOne(u->name());
    usernames();
    delete u;
}
