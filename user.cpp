#include "user.h"

User::User(QTcpSocket * socket)// : _outputDataStream(&_byteArray, QIODevice::WriteOnly)
{
    _socket = socket;
    newStream();
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
    newStream();
}
