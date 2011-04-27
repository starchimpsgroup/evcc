#include "login.h"
#include "ui_login.h"
#include <QMessageBox>

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    _login = Login::WRONGINPUT;
}

Login::~Login()
{
    delete ui;
}

void Login::on_buttonBox_accepted()
{
    _name   = ui->name->text();
    _server = ui->server->text();
    _port   = ui->port->text().toInt();

    if(server().isEmpty())
    {
        QMessageBox::warning(this, tr("Input error"),
                                   tr("Please enter a server."));
    }
    else if(port() == 0)
    {
        QMessageBox::warning(this, tr("Input error"),
                                   tr("Please enter a port."));
    }
    else if(name().isEmpty())
    {
        QMessageBox::warning(this, tr("Input error"),
                                   tr("Please enter a name."));
    }
    else
    {
        _login = Login::OK;
    }
}

void Login::on_buttonBox_rejected()
{
    _login = Login::CANCEL;
}
