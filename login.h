#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>

namespace Ui {
    class Login;
}

//! The Login class.
/*!
    Login screen.
 */
class Login : public QDialog
{
    Q_OBJECT

public:
    //! Constructor Login class.
    /*!
        \param parent Parent widget
     */
    explicit Login(QWidget *parent = 0);

    //! Destructor Login class.
    ~Login();

    //! Login typ enumeration.
    /*!
        Different typs of the login window
     */
    typedef enum LoginTyp
    {
        //! Ok typ.
        /*!
            All entries accepted
         */
        OK,

        //! Cancel typ.
        /*!
            User abort
         */
        CANCEL,

        //! Wrong input typ.
        /*!
            Not all entries accepted
         */
        WRONGINPUT
    }
    LoginTyp;

    //! Name.
    /*!
        Username
        \return Name of user
     */
    QString         name()  { return _name;   }

    //! Server url.
    /*!
        Server name or ip
        \return Server name ore ip
     */
    QString         server(){ return _server; }

    //! Port number.
    /*!
        Server port
        \return Server port number
     */
    quint16         port()  { return _port;   }

    //! Login typ.
    /*!
        Typ of login data
        \see LoginTyp
        \return Login typ
     */
    Login::LoginTyp login() { return _login;  }

private:
    Ui::Login      * ui;     //!< The UI
    QString         _name;   //!< Username
    QString         _server; //!< Server name or ip
    quint16         _port;   //!< Server port
    Login::LoginTyp _login;  //!< Login typ \see LoginTyp

private slots:
    //! Accept entries.
    /*!
        Ok button
     */
    void on_buttonBox_accepted();

    //! Reject entries.
    /*!
        Cancel button
     */
    void on_buttonBox_rejected();
};

#endif // LOGIN_H
