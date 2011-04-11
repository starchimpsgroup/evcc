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

        while(login.login() == Login::WRONGINPUT)
        {
            login.exec();

            if(login.login() == Login::OK)
            {
                _client = new Client(login.server(), login.port(), login.name(), this);
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
