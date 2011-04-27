#ifndef USER_H
#define USER_H

#include <QTcpSocket>
#include <QtCrypto>

class User
{
public:
    User(QTcpSocket * socket);
    ~User();
    QDataStream * outputDataStream(){ return _outputDataStream; }
    void send();
    void setData(QString name, QString key){ _name = name; setPublicKey(key); }

    QString name()            { return _name; }
    void setName(QString name){ _name = name; }

    QCA::PrivateKey privateKey()           { return _privateKey; }
    void setPrivateKey(QCA::PrivateKey key){ _privateKey = key;  }

    QCA::PublicKey publicKey()            { return _publicKey; }
    void setPublicKey(QString key);
    void setPublicKey(QCA::PublicKey key) { _publicKey = key;  }

    bool isCalling(){ return _calling; }
    User * partner(){ return _callingPartner; }

    void setCalling( User * partner ){ _calling = true; _callingPartner = partner; }
    void endCalling(){ _calling = false; }

    quint32 &blockSize(){ return _blockSize; }

    static QCA::PublicKey publicKeyFromString(QString key);

private:
    void newStream();

private:
    QString         _name;
    QByteArray      _byteArray;
    QDataStream   * _outputDataStream;
    QTcpSocket    * _socket;
    QCA::PrivateKey _privateKey;
    QCA::PublicKey  _publicKey;
    bool            _calling;
    User          * _callingPartner;
    quint32         _blockSize;
};

#endif // USER_H
