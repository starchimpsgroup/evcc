#ifndef CHOICE_H
#define CHOICE_H

#include <QWidget>

namespace Ui {
    class Choice;
}

class Choice : public QWidget
{
    Q_OBJECT

public:
    explicit Choice(QWidget *parent = 0);
    ~Choice();

    typedef enum ContentPage
    {
        CLIENT,                            //!< Client
        SERVER,                            //!< Server

    }
    ContentPage;

private:
    Ui::Choice *ui;

private slots:
    void on_client_clicked(){ emit changeContent(Choice::CLIENT); }
    void on_server_clicked(){ emit changeContent(Choice::SERVER); }

signals:
    void changeContent(Choice::ContentPage content);
};

#endif // CHOICE_H
