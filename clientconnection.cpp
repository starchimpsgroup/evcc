#include "clientconnection.h"

ClientConnection::ClientConnection(QString server, quint16 port, QString userName, QObject *parent) :
    QObject(parent)
{
    _state  = ClientConnection::IDLE;

    _server = server;
    _port   = port;

    _socket = new QTcpSocket(this);
    _socket->connectToHost(_server, _port);

    _user = new User(_socket);
    _user->setName(userName);

    QObject::connect(_socket, SIGNAL(readyRead()), this, SLOT(read()));
    QObject::connect(_socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    QObject::connect(_socket, SIGNAL(error(QAbstractSocket::SocketError)),
                        this, SLOT(displayError(QAbstractSocket::SocketError)));

    _audioDataSize = 0;
    _audioDataPacket.clear();

    QCA::init();
}

ClientConnection::~ClientConnection()
{
    delete _user;
}

void ClientConnection::disconnected()
{
    _socket->blockSignals(true);
    emit message(tr("Disconnected from server"), ServerMessages::ERROR);
}

void ClientConnection::call(QString name)
{
    _userCalling = name;
    *_user->outputDataStream() << connectionTyp(ServerConnectionTyps::CALL) << name;
    _user->send();
}

void ClientConnection::callEnd()
{
    *_user->outputDataStream() << connectionTyp(ServerConnectionTyps::CALLEND);
    _user->send();
}

void ClientConnection::callAccept()
{
    *_user->outputDataStream() << connectionTyp(ServerConnectionTyps::CALLACCEPTED);
    _user->send();
}

void ClientConnection::sendAudioData(QByteArray audioData)
{
    audioData = audioData.right(audioData.size()-44);
    QByteArray send;

    //qDebug(qPrintable("KeyLen: " + QString::number(_users[_userCalling].size()))); // ###

    *_user->outputDataStream() << connectionTyp(ServerConnectionTyps::AUDIODATASIZE) << (qint32)(audioData.size());
    _user->send();

    while(!audioData.isEmpty())
    {
        if(audioData.size() > 86)
        {
            send = audioData.left(86);
            audioData.remove(0, 86);
        }
        else
        {
            send = audioData;
            audioData.clear();
        }

        QCA::SecureArray result = User::publicKeyFromByteArray(_users[_userCalling]).encrypt(send, QCA::EME_PKCS1_OAEP);
        if(result.isEmpty()) {
            emit message(tr("Error encrypting"), ServerMessages::ERROR);
            return;
        }

        qDebug(qPrintable("audioDataEncryptLen: " + QString::number(result.toByteArray().size()))); // ###

        *_user->outputDataStream() << connectionTyp(ServerConnectionTyps::AUDIODATA) << result.toByteArray();
        _user->send();
    }
}

void ClientConnection::read()
{
    QDataStream in(_socket);
    QDataStream &out = *_user->outputDataStream();
    in.setVersion(QDataStream::Qt_4_0);

    if (_user->blockSize() == 0) {
        if (_socket->bytesAvailable() < (int)sizeof(qint32))
            return;

        in >> _user->blockSize();
    }

    if (_socket->bytesAvailable() < _user->blockSize())
        return;

    _user->blockSize() = 0;

    qint32 typ;
    in >> typ;

    switch((ServerConnectionTyps::ConnectionTyp)typ)
    {
    case ServerConnectionTyps::CONNECTIONACCEPTED:
    {
        if(!QCA::isSupported("pkey") || !QCA::PKey::supportedIOTypes().contains(QCA::PKey::RSA))
        {
            emit message(tr("RSA is not supported!"), ServerMessages::ERROR);
            return;
        }
        else
        {
            _user->setPrivateKey(QCA::KeyGenerator().createRSA(1024));
            if(_user->privateKey().isNull())
            {
                emit message(tr("Failed to make private RSA key"), ServerMessages::ERROR);
                return;
            }

            _user->setPublicKey(_user->privateKey().toPublicKey());

            if(!_user->publicKey().canEncrypt()) {
                emit message(tr("This kind of key cannot encrypt"), ServerMessages::ERROR);
                return;
            }

            out << connectionTyp(ServerConnectionTyps::USERNAME) << _user->name() << _user->publicKey().toDER();
            _user->send();
        }
        break;
    }
    case ServerConnectionTyps::USERNAMEACCEPTED:
    {
        emit connectionEstablished();
        break;
    }
    case ServerConnectionTyps::USERNAMEDENIED:
    {
        //_error = true;
        emit message(tr("User with this name is already logged in"), ServerMessages::ERROR);
        return;
        break;
    }
    case ServerConnectionTyps::USERNAMES:
    {
        _users.clear();
        QString name;
        QByteArray key;

        quint32 len;
        in >> len;

        for(uint i = 0; i < len; i++)
        {
            in >> name >> key;

            if(name != _user->name())
                _users.insert(name, key);
        }

        emit userListRefresh();

        break;
    }
    case ServerConnectionTyps::CALLEND:
    {
        // if partner/server terminates
        emit callTerminated();
        _state  = ClientConnection::IDLE;
        break;
    }
    case ServerConnectionTyps::CALLACCEPTED:
    {
        // wait for calletablished
        emit callOut(_userCalling);
        _state  = ClientConnection::CALLING;
        break;
    }
    case ServerConnectionTyps::CALLDENIED:
    {
        // if partner is calling
        emit callDenied(_userCalling);
        _state  = ClientConnection::IDLE;
        break;
    }
    case ServerConnectionTyps::CALL:
    {
        // callacept / denied
        QString name;
        in >> name;
        _userCalling = name;
        emit callIn(name);
        _state  = ClientConnection::INCOMINGCALL;
        break;
    }
    case ServerConnectionTyps::CALLESTABLISHED:
    {
        emit callEstablished();
        _state  = ClientConnection::CALLING;
        break;
    }
    case ServerConnectionTyps::AUDIODATA:
    {
        QByteArray data;
        in >> data;

        QCA::SecureArray decrypt;
        if(!_user->privateKey().decrypt(data, &decrypt, QCA::EME_PKCS1_OAEP))
        {
            emit message(tr("Error decrypting"), ServerMessages::ERROR);
            return;
        }

        qDebug(qPrintable("audioDataDecryptLen: " + QString::number(decrypt.toByteArray().size()))); // ###

        if(_audioDataSize != _audioDataPacket.size())
        {
            _audioDataPacket.append(decrypt.toByteArray());
        }
        else
        {
            emit receivedSoundData(_audioDataPacket);
            _audioDataSize = 0;
            _audioDataPacket.clear();
        }
        break;
    }
    case ServerConnectionTyps::AUDIODATATRANSFERRED:
    {
        emit dataTransferred();
        break;
    }
    case ServerConnectionTyps::AUDIODATASIZE:
    {
        in >> _audioDataSize;
        break;
    }
    default:
        emit message(tr("Undefined connection typ"), ServerMessages::WARNING);
        return;
    }

    if(!in.atEnd())
    {
        read();
    }
}

void ClientConnection::displayError(QAbstractSocket::SocketError socketError)
{
    //_error = true;

    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        emit message( tr("The host was not found. "
                         "Please check the host name and port settings."),
                     ServerMessages::ERROR);
        break;
    case QAbstractSocket::ConnectionRefusedError:
        emit message( tr("The connection was refused by the peer. "
                         "Make sure the server is running, "
                         "and check that the host name and port "
                         "settings are correct."),
                     ServerMessages::ERROR);
        break;
    default:
        emit message( tr("The following error occurred: %1.")
                     .arg(_socket->errorString()),
                     ServerMessages::ERROR);
    }
}
