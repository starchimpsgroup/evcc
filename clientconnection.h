#ifndef CLIENTCONNECTION_H
#define CLIENTCONNECTION_H

#include <QObject>
#include <QTcpSocket>
#include <QtCrypto>
#include "servermessages.h"
#include "serverconnectiontyps.h"
#include "user.h"

//! The ClientConnection class.
/*!
    ClientConnection for the clients.
 */
class ClientConnection : public QObject
{
    Q_OBJECT
public:
    //! Constructor ClientConnection class.
    /*!
        Initialize connection to the Server, connect singals and slots.
        \param server Server name or ip
        \param port Server port
        \param userName Client username
        \param parent Parent widget
     */
    explicit ClientConnection(QString server, quint16 port, QString userName, QObject *parent = 0);

    //! Destructor ClientConnection class.
    ~ClientConnection();

    //! Returns usernames.
    /*!
        List of usernames
        \return List of users
     */
    QList<QString> users(){ return _users.keys(); }

    //! Call.
    /*!
        Call a user with specified name
        \param name Name to call
     */
    void call(QString name);

    //! Call end.
    /*!
        Terminates call
     */
    void callEnd();

    //! Call accept.
    /*!
        Accept incomming call
     */
    void callAccept();

    //! Send audio data.
    /*!
        Send encrypted audio packet to the callingpartner
        \param audioData Data to send
     */
    void sendAudioData(QByteArray audioData);

    //! Connection state enumeration.
    /*!
        Different states of the connection
     */
    typedef enum ConnectionState
    {
        //! Idle state.
        /*!
            No call active
         */
        IDLE,

        //! Incoming call state.
        /*!
            Someone is calling
         */
        INCOMINGCALL,

        //! Calling state.
        /*!
            Active call
         */
        CALLING
    }
    ConnectionState;

    //! State of connection.
    /*!
        Returns the actual state of the connection
        \return Connection state
     */
    ClientConnection::ConnectionState state(){ return _state; }

private:
    //! Convert connection typ.
    /*!
        Convert connection typ to qint32, for network sending
        \return qint32 castet typ
        \param t Int connection typ
     */
    qint32 connectionTyp(ServerConnectionTyps::ConnectionTyp t){ return (qint32)t; }

private:
    QTcpSocket               * _socket;          //!< Socket for serverconnection
    QString                    _server;          //!< Serveradress or ip
    quint16                    _port;            //!< Server port number

    User                     * _user;            //!< User specific informations \see User
    QHash<QString, QByteArray> _users;           //!< Users on the server, with name and public key
    QString                    _userCalling;     //!< Name of callingpartner

    qint32                     _audioDataSize;   //!< Size of incomming audio data packet
    QByteArray                 _audioDataPacket; //!< Incomming audio data packet

    ClientConnection::ConnectionState _state;    //!< State of the connection \see ConnectionState

signals:
    //! Message.
    /*!
        Shows messagebox with specified text and typ
        \see ServerMessages::MessageTyp
        \param text Message text
        \param typ Message typ
     */
    void message(QString, ServerMessages::MessageTyp);

    //! Connection established.
    /*!
        Connection to the server is ready, username accepted
     */
    void connectionEstablished();

    //! User list refresh.
    /*!
        New list of usernames received
     */
    void userListRefresh();

    //! Call out.
    /*!
        Outgoing call, if callingpartner is not calling
        \param user User called out user
     */
    void callOut(QString);

    //! Call in.
    /*!
        Incomming call from callingpartner
        \param user Called in user
     */
    void callIn(QString);

    //! Call denied.
    /*!
        Callingpartner is calling another person, more than one connection is not allowed
        \param user Denied user
     */
    void callDenied(QString);

    //! Call terminated.
    /*!
        Callingpartner terminate call ore server is down
     */
    void callTerminated();

    //! Call established.
    /*!
        Call is established and audio data will be transfered
     */
    void callEstablished();

    //! Received sound data.
    /*!
        Incomming date from connectionpartner
        \param date Received data
     */
    void receivedSoundData(QByteArray);

    //! Data transferred.
    /*!
        Data packet completely transferred
     */
    void dataTransferred();

public slots:
    //! Read input and output connection transfer data.
    /*!
        Switch between different connectiontypes and process the data
     */
    void read();

    //! Display error.
    /*!
        Displays error text on the server log
        \param socketError Error of socket
     */
    void displayError(QAbstractSocket::SocketError socketError);

    //! Disconnected.
    /*!
        Data packet completely transferred
     */
    void disconnected();
};

#endif // CLIENTCONNECTION_H
