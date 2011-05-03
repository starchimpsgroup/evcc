#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>

//! The Settings class.
/*!
    Settings of program.
 */
class Settings
{
public:
    //! Constructor Settings class.
    /*!
        Settings of program
     */
    Settings();

    //! Destructor Settings class.
    ~Settings(){ _settings.sync(); }

    //! Set server port.
    /*!
        Set server port.
        \param port Server port
     */
    void setPort        (QString port  ){ _settings.setValue("server/port",         port);   }

    //! Set input device.
    /*!
        Set audio input device.
        \param device Input device
     */
    void setInputDevice (QString device){ _settings.setValue("client/inputDevice",  device); }

    //! Set output device.
    /*!
        Set audio output device.
        \param device Output device
     */
    void setOutputDevice(QString device){ _settings.setValue("client/outputDevice", device); }

    //! Get port number
    /*!
        Returns server port number
        \return Server port number
     */
    QString port()        { return _settings.value("server/port",         "60886").toString(); }

    //! Get input device
    /*!
        Returns audio input device
        \return Audio input device
     */
    QString inputDevice() { return _settings.value("client/inputDevice",  "pulse").toString(); }

    //! Get output device
    /*!
        Returns audio output device
        \return Audio output device
     */
    QString outputDevice(){ return _settings.value("client/outputDevice", "pulse").toString(); }

private:
    QSettings _settings; //!< Settings of program
};

#endif // SETTINGS_H
