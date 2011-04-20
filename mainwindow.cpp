#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "login.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    _playRing    = new PlayFile(":/sounds/ring.wav");
    _playRingPH  = new PlayFile(":/sounds/ringph.wav");
    _playOffHook = new PlayFile(":/sounds/offhook.wav");

    setChoice();
}

MainWindow::~MainWindow()
{
    delete _playRing;
    delete _playRingPH;
    delete _playOffHook;
    delete ui;
}

void MainWindow::setChoice()
{
    ui->statusBar->clearMessage();

    _choice = new Choice(this);
    QObject::connect(_choice, SIGNAL(changeContent(Choice::ContentPage)), this, SLOT(setPage(Choice::ContentPage)));

    setCentralWidget(_choice);
}

void MainWindow::serverError()
{
    stopSound();
    setChoice();
    qDebug("ServerError");
}

void MainWindow::connectionEstablished()
{
    setCentralWidget(_client);
}

void MainWindow::connectionLost()
{
    stopSound();
    setChoice();
    qDebug("ConnectionLost");
}

void MainWindow::stopSound()
{
    _playRing->stop();
    _playRingPH->stop();
    _playOffHook->stop();
}

void MainWindow::callOut(QString name)
{
    ui->statusBar->showMessage("Calling " + name + "...");
    _playRingPH->play();
}

void MainWindow::callIn(QString name)
{
    ui->statusBar->showMessage("Calling " + name + "...");
    _playRing->play();
}

void MainWindow::callDenied(QString name)
{
    ui->statusBar->showMessage(name + " is calling...");
    _playOffHook->play();
}

void MainWindow::callTerminated()
{
    ui->statusBar->showMessage("Call Terminated");
    stopSound();
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
                QObject::connect(_client, SIGNAL(serverError()), this, SLOT(serverError()));
            }

        }
    }
    else if(page == Choice::SERVER)
    {
        _server = new Server(this);
        setCentralWidget(_server);
    }
}
