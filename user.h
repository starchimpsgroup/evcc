#ifndef USER_H
#define USER_H

#include <QTcpSocket>

class User
{
public:
    User(QTcpSocket * socket);
    QDataStream * outputDataStream(){ return &_outputDataStream; }
    void send();
    QString name()            { return _name; }
    void setName(QString name){ _name = name; }

private:
    QString      _name;
    QByteArray   _byteArray;
    QDataStream  _outputDataStream;
    QTcpSocket * _socket;
};

#endif // USER_H
