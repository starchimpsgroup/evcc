#ifndef SERVERCONNECTIONTYPS_H
#define SERVERCONNECTIONTYPS_H

#include <QObject>

class ServerConnectionTyps : public QObject
{
    Q_OBJECT
public:
    typedef enum ConnectionTyp
    {
        CONNECTIONACCEPTED,
        USERNAME,
        USERNAMEACCEPTED,
        USERNAMEDENIED,
        USERNAMES,
        CALL,
        CALLACCEPTED,
        CALLDENIED,
        CALLEND,
        CALLESTABLISHED,
        AUDIODATA,
        AUDIODATATRANSFERRED
    }
    ConnectionTyp;

};

#endif // SERVERCONNECTIONTYPS_H
