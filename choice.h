#ifndef CHOICE_H
#define CHOICE_H

#include <QWidget>

namespace Ui {
    class Choice;
}

//! The Choice class.
/*!
    Select Client or Server.
 */
class Choice : public QWidget
{
    Q_OBJECT

public:

    //! Constructor Choice class
    /*!
        \param parent Parent widget
     */
    explicit Choice(QWidget *parent = 0);

    //! Destructor Choice class.
    ~Choice();

    //! Content page enumeration.
    /*!
        Types of the different windows
     */
    typedef enum ContentPage
    {
        CLIENT, //!< Client window
        SERVER  //!< Server window

    }
    ContentPage;

private:
    Ui::Choice * ui; //!< The UI

private slots:

    //! Client button slot.
    /*!
        Shows the client window
        \see ContentPage
        \see changeContent
     */
    void on_client_clicked(){ emit changeContent(Choice::CLIENT); }

    //! Server button slot.
    /*!
        Shows the server window
        \see ContentPage
        \see changeContent
     */
    void on_server_clicked(){ emit changeContent(Choice::SERVER); }

signals:
    //! Signal for content change.
    /*!
        Signal to shows the different windows
        \param content Content page
        \see ContentPage
     */
    void changeContent(Choice::ContentPage content);
};

#endif // CHOICE_H
