#ifndef SERVERMESSAGES_H
#define SERVERMESSAGES_H

#include <QObject>

class ServerMessages : public QObject
{
    Q_OBJECT
public:

    typedef enum MessageTyp
    {
        ERROR,
        TEXT,
        SEND,
    }
    MessageTyp;
};

#endif // SERVERMESSAGES_H
