#ifndef USER_H
#define USER_H

#include <QTcpSocket>

class User
{
public:
    User(QTcpSocket * socket);
    QDataStream * outputDataStream(){ return _outputDataStream; }
    void send();
    QString name()            { return _name; }
    void setName(QString name){ _name = name; }

    QString privateKey()           { return _privateKey; }
    void setPrivateKey(QString key){ _privateKey = key;  }

    QString publicKey()            { return _publicKey; }
    void setPublicKey(QString key) { _publicKey = key;  }

private:
    void newStream();

private:
    QString       _name;
    QByteArray    _byteArray;
    QDataStream * _outputDataStream;
    QTcpSocket  * _socket;
    QString       _privateKey;
    QString       _publicKey;
};

#endif // USER_H
