#ifndef USER_H
#define USER_H

#include <QTcpSocket>

class User
{
public:
    User(QTcpSocket * socket);
    QDataStream * outputDataStream(){ return &_outputDataStream; }
    void send();

private:
    QByteArray   _byteArray;
    QDataStream  _outputDataStream;
    QTcpSocket * _socket;
};

#endif // USER_H
