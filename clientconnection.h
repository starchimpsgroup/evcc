#ifndef CLIENTCONNECTION_H
#define CLIENTCONNECTION_H

#include <QObject>
#include <QTcpSocket>
#include "servermessages.h"

class ClientConnection : public QObject
{
    Q_OBJECT
public:
    explicit ClientConnection(QString server, quint16 port, QObject *parent = 0);

private:
    QTcpSocket * _socket;
    QString      _server;
    quint16      _port;

signals:
    void message(QString text, ServerMessages::MessageTyp typ);

public slots:
    void read();
    void displayError(QAbstractSocket::SocketError socketError);

};

#endif // CLIENTCONNECTION_H
