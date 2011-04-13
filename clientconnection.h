#ifndef CLIENTCONNECTION_H
#define CLIENTCONNECTION_H

#include <QObject>
#include <QTcpSocket>
#include "servermessages.h"
#include "serverconnectiontyps.h"
#include "user.h"

class ClientConnection : public QObject
{
    Q_OBJECT
public:
    explicit ClientConnection(QString server, quint16 port, QString userName, QObject *parent = 0);
    ~ClientConnection();

private:
    QTcpSocket * _socket;
    QString      _server;
    quint16      _port;
    User       * _user;
    QHash<QString, QString> _users; // name, key

private:
    qint32 connectionTyp(ServerConnectionTyps::ConnectionTyp t){ return (qint32)t; }

signals:
    void message(QString, ServerMessages::MessageTyp);
    void connectionEstablished();
    void connectionLost();

public slots:
    void read();
    void displayError(QAbstractSocket::SocketError socketError);

};

#endif // CLIENTCONNECTION_H
