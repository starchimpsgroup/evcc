#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>
#include <QStringList>
#include "servermessages.h"
#include "serverconnectiontyps.h"
#include "user.h"

//! The ServerConnection class.
/*!
    Manage the data transfer between clients.
 */
class ServerConnection : public QTcpServer
{
    Q_OBJECT

public:
    //! Constructor ServerConnection class.
    /*!
        Start listen
        \param port Server port
        \param host Listen on all network interfaces
        \param parent Parent widget
     */
    ServerConnection(quint16 port, QHostAddress host = QHostAddress::Any, QObject* parent = 0);

    //! Server status.
    /*!
        Server status after constructor
     */
    void serverStatus(){ emit message(_serverStatus, _serverStatusTyp); }

private:
    QHostAddress               _hostAddress;     //!< Listen on all interfaces
    quint16                    _port;            //!< Server port
    QString                    _serverStatus;    //!< Status of server
    ServerMessages::MessageTyp _serverStatusTyp; //!< Typ of server status \see ServerMessages::MessageTyp

    QHash<QTcpSocket*, User*>  _users;           //!< Hash of users \see User
    QStringList                _usernames;       //!< Username list

private:
    //! Incoming connection.
    /*!
        Incomming new client connection
        \param socketDescriptor Socket Descriptor
     */
    void incomingConnection ( int socketDescriptor );

    //! Is name existent.
    /*!
        Checks existent usernames
        \param name Name of new user
        \return Is user existent
     */
    bool isNameExistent(QString name);

    //! Get user by name.
    /*!
        Returns user by specified name
        \param name Name of new user
        \return Is user existent
        \see User
     */
    User * getUserByName(QString name);

    //! Convert connection typ.
    /*!
        Convert connection typ to qint32, for network sending
        \return qint32 castet typ
        \param t Int connection typ
     */
    qint32 connectionTyp(ServerConnectionTyps::ConnectionTyp t){ return (qint32)t; }

    //! Usernames.
    /*!
        Send usernames with public key to clients
     */
    void usernames();

    //! Call end.
    /*!
        Terminates call between two clients
        \param u User who terminates call
        \see User
     */
    void callEnd(User * u);

    //! Clear.
    /*!
        Clear transaction data from user and calling partner
        \param u User
        \see User
     */
    void clear(User * u);

private slots:
    //! Socket ready read.
    /*!
        Manage client network actions.\n
        Get the connection typ and switch between actions for the specified typ.
        \see ServerConnectionTyps::ConnectionTyp
     */
    void socketReadyRead();

    //! Socket disconnected.
    /*!
        Remove diconnected user
        \see User
     */
    void socketDisconnected();

signals:
    //! Server message.
    /*!
        Messages for the serverlog with specified typ.
        \param text Text of message
        \param typ Typ of message
        \see ServerMessages::MessageTyp
     */
    void message(QString text, ServerMessages::MessageTyp typ);
};

#endif // TCPSERVER_H
