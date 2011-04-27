#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>

namespace Ui {
    class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = 0);
    ~Login();

    typedef enum LoginTyp
    {
        OK,
        CANCEL,
        WRONGINPUT
    }
    LoginTyp;

    QString         name()  { return _name;   }
    QString         server(){ return _server; }
    quint16         port()  { return _port;   }
    Login::LoginTyp login() { return _login;  }

private:
    Ui::Login         * ui;
    QString            _name;
    QString            _server;
    quint16            _port;
    Login::LoginTyp    _login;

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
};

#endif // LOGIN_H
