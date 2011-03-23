#include "serverconnection.h"
#include <QTcpSocket>
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

    _serverStatus = tr("Server started.");
    _serverStatusTyp = ServerMessages::TEXT;

    connect( this, SIGNAL(newConnection()), this, SLOT(send()) );
};

void ServerConnection::send()
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);

    out << (quint16)0;
    out << QString("test");
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
}