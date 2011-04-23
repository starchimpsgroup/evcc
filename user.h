#ifndef USER_H
#define USER_H

#include <QTcpSocket>

class User
{
public:
    User(QTcpSocket * socket);
    ~User();
    QDataStream * outputDataStream(){ return _outputDataStream; }
    void send();
    void setData(QString name, QString key){ _name = name; _publicKey = key; }

    QString name()            { return _name; }
    void setName(QString name){ _name = name; }

    QString privateKey()           { return _privateKey; }
    void setPrivateKey(QString key){ _privateKey = key;  }

    QString publicKey()            { return _publicKey; }
    void setPublicKey(QString key) { _publicKey = key;  }

    bool isCalling(){ return _calling; }
    User * partner(){ return _callingPartner; }

    void setCalling( User * partner ){ _calling = true; _callingPartner = partner; }
    void endCalling(){ _calling = false; }

    quint32 &blockSize(){ return _blockSize; }

private:
    void newStream();

private:
    QString       _name;
    QByteArray    _byteArray;
    QDataStream * _outputDataStream;
    QTcpSocket  * _socket;
    QString       _privateKey;
    QString       _publicKey;
    bool          _calling;
    User        * _callingPartner;
    quint32       _blockSize;
};

#endif // USER_H
