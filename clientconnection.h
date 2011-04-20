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
    QList<QString> users(){ return _users.keys(); }
    void call(QString name);
    void callEnd();

    typedef enum ConnectionState
    {
        IDLE,
        INCOMINGCALL,
        CALLING
    }
    ConnectionState;

private:
    QTcpSocket * _socket;
    QString      _server;
    quint16      _port;
    User       * _user;
    QHash<QString, QString> _users; // name, key
    QString      _userCalling;
    //bool         _error;

private:
    qint32 connectionTyp(ServerConnectionTyps::ConnectionTyp t){ return (qint32)t; }

signals:
    void message(QString, ServerMessages::MessageTyp);
    void connectionEstablished();
    void connectionLost();
    void userListRefresh();
    void callOut(QString);
    void callIn(QString);
    void callDenied(QString);
    void callTerminated();

public slots:
    void read();
    void displayError(QAbstractSocket::SocketError socketError);
    void disconnected();

};

#endif // CLIENTCONNECTION_H
