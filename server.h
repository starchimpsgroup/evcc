#ifndef SERVER_H
#define SERVER_H

#include <QWidget>
#include "serverconnection.h"
#include "servermessages.h"

namespace Ui {
    class Server;
}

class Server : public QWidget
{
    Q_OBJECT

public:
    explicit Server(QWidget *parent = 0);
    ~Server();

private:
    Ui::Server        * ui;
    ServerConnection  * _tcpServer;
    QString             _serverLog;

public slots:
    void addMessage(QString text, ServerMessages::MessageTyp typ);
};

#endif // SERVER_H
