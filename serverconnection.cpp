#include "serverconnection.h"
#include <QMessageBox>

ServerConnection::ServerConnection(quint16 port, QHostAddress host, QObject* parent) :
    QTcpServer(parent), _hostAddress(host), _port(port)
{
    if ( !listen( _hostAddress, _port ) )
    {
        _serverStatus    = tr("Unable to start the server: %1.")
                           .arg(this->errorString());
        _serverStatusTyp = ServerMessages::ERROR;

        return;
    }

    _serverStatus    = tr("Server started.");
    _serverStatusTyp = ServerMessages::TEXT;

    //connect( this, SIGNAL(newConnection()), this, SLOT(send()) );
};

bool ServerConnection::isNameExistent(QString name)
{
    if(!_users.isEmpty())
    {
        QHashIterator<QTcpSocket*, User*> i(_users);
        while (i.hasNext())
        {
            i.next();

            if(i.value()->name() == name)
            {
                return true;
            }
        }
    }

    return false;
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

void ServerConnection::socketReadyRead()
{
    QTcpSocket * socket = (QTcpSocket *)QObject::sender();
    User * u = _users[socket];

    quint32 blockSize;
    QDataStream in(socket);
    QDataStream &out = *u->outputDataStream();
    in.setVersion(QDataStream::Qt_4_0);

    //if (blockSize == 0) {
    if (socket->bytesAvailable() < (int)sizeof(quint32))
        return;

    in >> blockSize;
    //}

    if (socket->bytesAvailable() < blockSize)
        return;

    qint32 typ;
    in >> typ;

    switch((ServerConnectionTyps::ConnectionTyp)typ)
    {
    case ServerConnectionTyps::USERNAMES:
        out << connectionTyp(ServerConnectionTyps::USERNAMES) << (quint32)_users.size();

        if(!_users.isEmpty())
        {
            QHashIterator<QTcpSocket*, User*> i(_users);
            while (i.hasNext())
            {
                i.next();

                out << i.value()->name() << i.value()->publicKey();
            }
        }

        break;

    case ServerConnectionTyps::USERNAME:

        QString name;
        in >> name;

        if(isNameExistent(name))
        {
            out << connectionTyp(ServerConnectionTyps::USERNAMEDENIED);
            // socketDisconnected
        }
        else
        {
            out << connectionTyp(ServerConnectionTyps::USERNAMEACCEPTED);
            u->setName(name);
        }
        break;
    }

    u->send();
}

void ServerConnection::socketDisconnected()
{
    QTcpSocket * socket = (QTcpSocket *)QObject::sender();
    User * u =  _users.take(socket);

    emit message(tr("Diconnected: ") + socket->peerAddress().toString() + ":"
                                     + QString::number(socket->peerPort()), ServerMessages::SEND);

    delete u;
    delete socket;
}

/*void ServerConnection::send()
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);

    out << (quint16)0;
    out << QString("test123456");
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));

    QTcpSocket * clientConnection = this->nextPendingConnection();
    emit message(tr("Send: ") + clientConnection->localAddress().toString() + ":"
                              + QString::number(clientConnection->localPort()) + " -> "
                              + clientConnection->peerAddress().toString() + ":"
                              + QString::number(clientConnection->peerPort()), ServerMessages::SEND);
    connect(clientConnection, SIGNAL(disconnected()), clientConnection, SLOT(deleteLater()));

    clientConnection->write(block);
    clientConnection->disconnectFromHost();
}*/
