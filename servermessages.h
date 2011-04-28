#ifndef SERVERMESSAGES_H
#define SERVERMESSAGES_H

//! The ServerMessages class.
/*!
    Messagetypes for the serverlog window.
 */
class ServerMessages
{
public:

    //! Message typ enumeration.
    /*!
        Types of the different messages
     */
    typedef enum MessageTyp
    {
        //! Error message.
        /*!
            Red color, server/client can not work
         */
        ERRORMESSAGE,

        //! Text message.
        /*!
            Black color, normal text
         */
        TEXT,

        //! Send message.
        /*!
            Blue color, for data transfers between client and server
         */
        SEND,

        //! Warning message.
        /*!
            Orange color, for incorret data transfers
         */
        WARNING,

        //! Information message.
        /*!
            Yellow color
         */
        INFORMATION
    }
    MessageTyp;
};

#endif // SERVERMESSAGES_H
