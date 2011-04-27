#include "user.h"

User::User(QTcpSocket * socket)// : _outputDataStream(&_byteArray, QIODevice::WriteOnly)
{
    _calling   = false;
    _socket    = socket;
    _blockSize = 0;
    newStream();
}

User::~User()
{
    delete _outputDataStream;
}

void User::newStream()
{
    _outputDataStream = new QDataStream(&_byteArray, QIODevice::WriteOnly);
    _outputDataStream->setVersion(QDataStream::Qt_4_0);
    *_outputDataStream << (quint32)0;
}

void User::send()
{
    _outputDataStream->device()->seek((quint32)0);
    *_outputDataStream << (quint32)(_byteArray.size() - sizeof(quint32));

    _socket->write(_byteArray);
    delete _outputDataStream;
    _byteArray.clear();
    newStream();
}

void User::setPublicKey(QByteArray &key)
{
    _publicKey = User::publicKeyFromByteArray(key);
}

QCA::PublicKey User::publicKeyFromByteArray(QByteArray &key)
{
    //qDebug(qPrintable("KeyLen: " + QString::number(key.size()))); // ###

    QCA::ConvertResult conversionResult;
    QCA::PublicKey publicKey = QCA::PublicKey::fromDER(key, &conversionResult);
    if (QCA::ConvertGood != conversionResult)
    {
        qDebug("Public key read failed");
    }

    return publicKey;
}
