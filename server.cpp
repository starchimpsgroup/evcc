#include "server.h"
#include "ui_server.h"
#include <QNetworkInterface>
#include <QTextEdit>

#define TCP_PORT 60886

Server::Server(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Server)
{
    ui->setupUi(this);

    ui->textEdit->setStyleSheet("background-color: white;"
                                "background-image: url(:/logos/serverLogo.png);"
                                "background-repeat: none;"
                                "background-position: center;");

    QString ipAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // use the first non-localhost IPv4 address
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
            ipAddressesList.at(i).toIPv4Address()) {
            ipAddress = ipAddressesList.at(i).toString();
            break;
        }
    }

    // if we did not find one, use IPv4 localhost
    if (ipAddress.isEmpty())
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();

    addMessage("IP-Address: " + ipAddress + "<br />Port: " + QString::number( TCP_PORT ), ServerMessages::TEXT);

    connect(_tcpServer = new ServerConnection( TCP_PORT ),
            SIGNAL(message(QString,ServerMessages::MessageTyp)),
            this,
            SLOT(addMessage(QString,ServerMessages::MessageTyp)));

    _tcpServer->serverStatus();
}

Server::~Server()
{
    delete _tcpServer;
    delete ui;
}

void Server::addMessage(QString text, ServerMessages::MessageTyp typ)
{
    if(typ == ServerMessages::ERROR)
    {
        _serverLog += "<span style=\"color:red;\">" + text + "</span><br />";
    }
    else if(typ == ServerMessages::SEND)
    {
        _serverLog += "<span style=\"color:blue;\">" + text + "</span><br />";
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
