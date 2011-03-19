#include "mainwindow.h"
#include "ui_mainwindow.h"

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
    delete _choice;
    delete _client;
    delete _server;
    delete ui;
}

void MainWindow::setPage(Choice::ContentPage page)
{
    if(page == Choice::CLIENT)
    {
        _client = new Client(this);
        setCentralWidget(_client);
    }
    else if(page == Choice::SERVER)
    {
        _server = new Server(this);
        setCentralWidget(_server);
    }
}
