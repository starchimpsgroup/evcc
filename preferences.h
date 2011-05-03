#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QDialog>
#include <QAudioDeviceInfo>

namespace Ui {
    class Preferences;
}

//! The Preferences class.
/*!
    Preferences screen.
 */
class Preferences : public QDialog
{
    Q_OBJECT

public:

    //! Constructor Preferences class.
    /*!
        \param port Server port number
        \param audioInputDevices List of input devices
        \param selectedInputDevice Device to select
        \param audioOutputDevices List of output devices
        \param selectedOutputDevice Device to select
        \param parent Parent widget
     */
    explicit Preferences(QString port,
                         QList<QAudioDeviceInfo> audioInputDevices,
                         QString selectedInputDevice,
                         QList<QAudioDeviceInfo> audioOutputDevices,
                         QString selectedOutputDevice,
                         QWidget *parent = 0);

    //! Destructor Preferences class.
    ~Preferences();

    //! Get server port number.
    /*!
        Returns Server port
        \return Server port
     */
    QString port()                { return _port;                 }

    //! Get selected input device
    /*!
        Returns selected input device
        \return Selected input device
     */
    QString selectedInputDevice() { return _selectedInputDevice;  }

    //! Get selected output device
    /*!
        Returns selected output device
        \return Selected output device
     */
    QString selectedOutputDevice(){ return _selectedOutputDevice; }

private:
    Ui::Preferences *ui;           //!< The UI

    QString _port;                 //!< Port of server
    QString _selectedInputDevice;  //!< Selected input device
    QString _selectedOutputDevice; //!< Selected output device

private slots:
    //! Save preferences.
    /*!
        Save preferences in variables
     */
    void save();

    //! Reset preferences.
    /*!
        Reset preferences
     */
    void reset();
};

#endif // PREFERENCES_H
