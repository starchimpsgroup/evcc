#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "choice.h"
#include "client.h"
#include "server.h"
#include "playfile.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void setChoice();

private:
    Ui::MainWindow * ui;

    Choice         * _choice;
    Client         * _client;
    Server         * _server;

    PlayFile       * _playRing;
    PlayFile       * _playRingPH;
    PlayFile       * _playOffHook;

private slots:
    void setPage(Choice::ContentPage page);
    void serverError();
    void connectionEstablished();
    void stopSound();
    void callOut(QString name);
    void callIn(QString name);
    void callDenied(QString name);
    void callTerminated();
    void callEstablished();
};

#endif // MAINWINDOW_H
