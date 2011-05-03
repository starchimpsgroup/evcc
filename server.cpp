#include "server.h"
#include "ui_server.h"
#include <QNetworkInterface>
#include <QTextEdit>

Server::Server(QString port, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Server)
{
    ui->setupUi(this);

    _port = port;

    ui->textEdit->setStyleSheet("background-color: white;"
                                "background-image: url(:/logos/serverLogo.png);"
                                "background-repeat: none;"
                                "background-position: center;");

    QString ipAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();

    /*!
        Use the first non-localhost IPv4 address\n
     */
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
            ipAddressesList.at(i).toIPv4Address()) {
            ipAddress = ipAddressesList.at(i).toString();
            break;
        }
    }

    /*!
        If we did not find one, use IPv4 localhost\n
     */
    if (ipAddress.isEmpty())
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();

    connect(_tcpServer = new ServerConnection( _port.toInt() ),
            SIGNAL(message(QString,ServerMessages::MessageTyp)),
            this,
            SLOT(addMessage(QString,ServerMessages::MessageTyp)));

    addMessage("IP-Address: " + ipAddress + "<br />Port: " + QString::number( _tcpServer->serverPort() ), ServerMessages::TEXT);

    _tcpServer->serverStatus();
}

Server::~Server()
{
    delete _tcpServer;
    delete ui;
}

void Server::addMessage(QString text, ServerMessages::MessageTyp typ)
{
    if(typ == ServerMessages::ERRORMESSAGE)
    {
        _serverLog += "<span style=\"color:red;\">" + text + "</span><br />";
    }
    else if(typ == ServerMessages::SEND)
    {
        _serverLog += "<span style=\"color:blue;\">" + text + "</span><br />";
    }
    else if(typ == ServerMessages::WARNING)
    {
        _serverLog += "<span style=\"color:orange;\">" + text + "</span><br />";
    }
    else if(typ == ServerMessages::TEXT)
    {
        _serverLog += "<span>" + text + "</span><br />";
    }

    ui->textEdit->setText(_serverLog);

    QTextCursor c = ui->textEdit->textCursor();
    c.movePosition(QTextCursor::End);
    ui->textEdit->setTextCursor(c);

}
