#ifndef SERVER_H
#define SERVER_H

#include <QWidget>
#include "serverconnection.h"
#include "servermessages.h"

namespace Ui {
    class Server;
}

//! The Server class.
/*!
    Server to connect clients.
 */
class Server : public QWidget
{
    Q_OBJECT

public:
    //! Constructor Server class.
    /*!
        Get serveradress and create server
        \param parent Parent widget
        \param port Server port number
     */
    explicit Server(QString port, QWidget *parent = 0);

    //! Destructor Server class.
    ~Server();

private:
    Ui::Server        * ui;         //!< The UI
    ServerConnection  * _tcpServer; //!< Tcp server \see ServerConnection
    QString             _serverLog; //!< Server log
    QString             _port;      //!< Server port number

public slots:
    //! Add message.
    /*!
        Adds text messages, with specified typ, to the server log
        \param text Message of text
        \param typ Typ of message
     */
    void addMessage(QString text, ServerMessages::MessageTyp typ);
};

#endif // SERVER_H
