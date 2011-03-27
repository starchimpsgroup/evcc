#include "login.h"
#include "ui_login.h"

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    _login = true;
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
}

void Login::on_buttonBox_rejected()
{
    _login = false;
}
