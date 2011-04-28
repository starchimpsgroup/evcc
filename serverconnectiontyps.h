#ifndef SERVERCONNECTIONTYPS_H
#define SERVERCONNECTIONTYPS_H

//! The ServerConnectionTyps class.
/*!
    Server connection typs for the server, client interaction.
 */
class ServerConnectionTyps
{
public:
    //! Connection typ enumeration.
    /*!
        Types of the different connections
     */
    typedef enum ConnectionTyp
    {
        //! Connection accepted.
        /*!
            Accepted connection to the server.
         */
        CONNECTIONACCEPTED,

        //! Username.
        /*!
            Transfer username to the server.\n
            Wait for username accepted or denied.
            \see USERNAMEACCEPTED
            \see USERNAMEDENIED
         */
        USERNAME,

        //! Username accepted.
        /*!
            Transfered if username is not in use.\n
            New userlist will be sendet to all clients.
            \see USERNAME
            \see USERNAMEDENIED
         */
        USERNAMEACCEPTED,

        //! Username denied.
        /*!
            Transfered if username is in use.
            \see USERNAME
            \see USERNAMEACCEPTED
         */
        USERNAMEDENIED,

        //! Usernames.
        /*!
            Get actual list of usernames.
            \see USERNAME
         */
        USERNAMES,

        //! Call.
        /*!
            Make a call to another client.\n
            Wait for call accept or denied signal.\n
            On accept the calling partner received a call signal, so the partner must accept ore end the call.\n
            On denied the calling partner is actually calling to someone else.
            \see CALLACCEPTED
            \see CALLDENIED
            \see CALLEND
         */
        CALL,

        //! Call accept.
        /*!
            The calling partner is free to call.\n
            The calling partner received a call signal, so the partner must accept ore end the call.
            \see CALL
            \see CALLDENIED
            \see CALLEND
         */
        CALLACCEPTED,

        //! Call denied.
        /*!
            the calling partner is actually calling to someone else.
            \see CALL
            \see CALLACCEPTED
         */
        CALLDENIED,

        //! Call end.
        /*!
            Terminates call between calling partners.\n
            Send both a call end signal.
         */
        CALLEND,

        //! Call established.
        /*!
            Send to both calling partners if a call is accepted by the partner.\n
            The audio data transfer beginns after this signal.
            \see CALLACCEPTED
            \see AUDIODATA
         */
        CALLESTABLISHED,

        //! Audio data.
        /*!
            Send the incomming audio data to the calling partner.\n
            Send sender audio data transfered for received data.
            \see AUDIODATATRANSFERRED
         */
        AUDIODATA,

        //! Audio data transferred.
        /*!
            Send after audio data is received.
            \see AUDIODATA
         */
        AUDIODATATRANSFERRED,

        //! Audio data size.
        /*!
            Packteparts of an audiofile.
         */
        AUDIODATASIZE
    }
    ConnectionTyp;

};

#endif // SERVERCONNECTIONTYPS_H
