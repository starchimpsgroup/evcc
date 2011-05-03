#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "login.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _playRing   (":/sounds/ring.wav"),
    _playRingPH (":/sounds/ringph.wav"),
    _playOffHook(":/sounds/offhook.wav", false)
{
    ui->setupUi(this);

    _preferences = new Preferences(_settings.port(),
                                   QAudioDeviceInfo::availableDevices(QAudio::AudioInput),
                                   _settings.inputDevice(),
                                   QAudioDeviceInfo::availableDevices(QAudio::AudioOutput),
                                   _settings.outputDevice(),
                                   this);

    setChoice();
}

MainWindow::~MainWindow()
{
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

    QObject * sender = QObject::sender();
    sender->disconnect();

    delete sender;

    qDebug("ServerError");
}

void MainWindow::connectionEstablished()
{
    setCentralWidget(_client);
}

void MainWindow::stopSound()
{
    _playRing.stop();
    _playRingPH.stop();
    _playOffHook.stop();
}

void MainWindow::callOut(QString name)
{
    ui->statusBar->showMessage("Calling " + name + "...");
    _playRingPH.play();
}

void MainWindow::callIn(QString name)
{
    ui->statusBar->showMessage("Calling " + name + "...");
    _playRing.play();
}

void MainWindow::callDenied(QString name)
{
    ui->statusBar->showMessage(name + " is calling...");
    _playOffHook.play();
}

void MainWindow::callTerminated()
{
    ui->statusBar->showMessage("Call Terminated");
    stopSound();
}

void MainWindow::callEstablished()
{
    ui->statusBar->showMessage("Call Established");
    stopSound();
}

void MainWindow::setPage(Choice::ContentPage page)
{
    if(page == Choice::CLIENT)
    {
        Login login(_preferences->port());

        while(login.login() == Login::WRONGINPUT)
        {
            login.exec();

            if(login.login() == Login::OK)
            {
                _client = new Client(login.server(),
                                     login.port(),
                                     _preferences->selectedInputDevice(),
                                     _preferences->selectedOutputDevice(),
                                     login.name(),
                                     this);
                QObject::connect(_client, SIGNAL(serverError()), this, SLOT(serverError()));
            }

        }
    }
    else if(page == Choice::SERVER)
    {
        _server = new Server(_preferences->port(), this);
        setCentralWidget(_server);
    }
}

void MainWindow::on_actionPreferences_triggered()
{
    _preferences->exec();

    _settings.setPort(_preferences->port());
    _settings.setInputDevice(_preferences->selectedInputDevice());
    _settings.setOutputDevice(_preferences->selectedOutputDevice());
}
