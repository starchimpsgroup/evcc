#include "user.h"

User::User(QTcpSocket * socket) : _outputDataStream(&_byteArray, QIODevice::WriteOnly)
{
    _socket = socket;
    _outputDataStream.setVersion(QDataStream::Qt_4_0);
    _outputDataStream << (quint16)0;
}

void User::send()
{
    _outputDataStream.device()->seek(0);
    _outputDataStream << (quint16)(_byteArray.size() - sizeof(quint16));

    _socket->write(_byteArray);
    _outputDataStream << (quint16)0;
}
