#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "choice.h"
#include "client.h"
#include "server.h"

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
    Ui::MainWindow * ui;

    Choice * _choice;
    Client * _client;
    Server * _server;

private slots:
    void setPage(Choice::ContentPage page);
};

#endif // MAINWINDOW_H
