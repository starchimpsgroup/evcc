#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "login.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    _choice = new Choice(this);
    QObject::connect(_choice, SIGNAL(changeContent(Choice::ContentPage)), this, SLOT(setPage(Choice::ContentPage)));

    setCentralWidget(_choice);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setPage(Choice::ContentPage page)
{
    if(page == Choice::CLIENT)
    {
        Login login;
        login.exec();

        if(login.login())
        {
            if(login.server().isEmpty())
            {
                QMessageBox::warning(this, tr("Input error"),
                                           tr("Please enter a server."));
            }
            else if(login.port() == 0)
            {
                QMessageBox::warning(this, tr("Input error"),
                                           tr("Please enter a port."));
            }
            else if(login.name().isEmpty())
            {
                QMessageBox::warning(this, tr("Input error"),
                                           tr("Please enter a name."));
            }
            else
            {
                _client = new Client(login.server(), login.port(), this);
                setCentralWidget(_client);
            }
        }
    }
    else if(page == Choice::SERVER)
    {
        _server = new Server(this);
        setCentralWidget(_server);
    }
}
