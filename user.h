#ifndef USER_H
#define USER_H

#include <QTcpSocket>
#include <QtCrypto>

//! The User class.
/*!
    User informations.
 */
class User
{
public:
    //! Constructor User class.
    /*!
        Generate new output data stream.
        \param socket Socket if the user
     */
    User(QTcpSocket * socket);

    //! Destructor User class.
    ~User();

    //! Output data stream.
    /*!
        Returns output data stream.
        \return Output data stream
     */
    QDataStream * outputDataStream(){ return _outputDataStream; }

    //! Send data.
    /*!
        Send data of the output data stream to the server.
     */
    void send();

    //! Set personal userdata.
    /*!
        Setter for username and private key.
        \param name Username
        \param key Private key
     */
    void setData(QString name, QByteArray key){ _name = name; setPublicKey(key); }

    //! Username.
    /*!
        Return username.
        \return Username
     */
    QString name()            { return _name; }

    //! Set name.
    /*!
        Setter for username.
        \param name Username
     */
    void setName(QString name){ _name = name; }

    //! Private key.
    /*!
        Return private key.
        \return Private key
     */
    QCA::PrivateKey privateKey()           { return _privateKey; }

    //! Set private key.
    /*!
        Set the private key.
        \param key Private key
     */
    void setPrivateKey(QCA::PrivateKey key){ _privateKey = key;  }

    //! Public key.
    /*!
        Return public key.
        \return Punblic key
     */
    QCA::PublicKey publicKey()            { return _publicKey; }

    //! Set public key.
    /*!
        Set the public key.
        \param key Public key
     */
    void setPublicKey(QByteArray &key);

    //! Set public key.
    /*!
        Set the public key.
        \param key Public key
     */
    void setPublicKey(QCA::PublicKey key) { _publicKey = key;  }

    //! Is calling.
    /*!
        Return calling value.
        \return Calling value
     */
    bool isCalling(){ return _calling; }

    //! Partner.
    /*!
        Return calling partner.
        \return Calling partner
        \see User
     */
    User * partner(){ return _callingPartner; }

    //! Set calling.
    /*!
        Set calling partner.
        \param partner Calling partner
        \see User
     */
    void setCalling( User * partner ){ _calling = true; _callingPartner = partner; }

    //! End calling.
    /*!
        Call terminated.
     */
    void endCalling(){ _calling = false; }

    //! Blocksize.
    /*!
        Return blocksize of the connection.
        \return Blocksize
     */
    quint32 &blockSize(){ return _blockSize; }

    //! Public key from byte array.
    /*!
        Return convertet byte array to public key.
        \return Covertet public key
        \param key Byte array public key
     */
    static QCA::PublicKey publicKeyFromByteArray(QByteArray &key);

private:
    //! New Stream.
    /*!
        Generates new output data stream.
     */
    void newStream();

private:
    QString         _name;             //!< Username
    QByteArray      _byteArray;        //!< Output byte array
    QDataStream   * _outputDataStream; //!< Outout data stream
    QTcpSocket    * _socket;           //!< Socket for server connection
    QCA::PrivateKey _privateKey;       //!< RSA private key
    QCA::PublicKey  _publicKey;        //!< RSA public key
    bool            _calling;          //!< Is calling value
    User          * _callingPartner;   //!< Calling partner \see User
    quint32         _blockSize;        //!< Blocksize of incommming packet
};

#endif // USER_H
