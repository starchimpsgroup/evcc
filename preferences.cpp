#include "preferences.h"
#include "ui_preferences.h"

Preferences::Preferences(QString port,
                         QList<QAudioDeviceInfo> audioInputDevices,
                         QString selectedInputDevice,
                         QList<QAudioDeviceInfo> audioOutputDevices,
                         QString selectedOutputDevice,
                         QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Preferences)
{
    ui->setupUi(this);

    QObject::connect(this, SIGNAL(accepted()), this, SLOT(save()));
    QObject::connect(this, SIGNAL(rejected()), this, SLOT(reset()));

    _port                 = port;
    _selectedInputDevice  = selectedInputDevice;
    _selectedOutputDevice = selectedOutputDevice;

    ui->port->setText(_port);

    foreach(QAudioDeviceInfo device, audioInputDevices)
    {
        ui->audioInput->addItem(device.deviceName());
    }

    ui->audioInput->setCurrentIndex(ui->audioInput->findText(_selectedInputDevice));

    foreach(QAudioDeviceInfo device, audioOutputDevices)
    {
        ui->audioOutput->addItem(device.deviceName());
    }

    ui->audioOutput->setCurrentIndex(ui->audioOutput->findText(_selectedOutputDevice));
}

Preferences::~Preferences()
{
    delete ui;
}

void Preferences::save()
{
    _port                 = ui->port->text();
    _selectedInputDevice  = ui->audioInput->currentText();
    _selectedOutputDevice = ui->audioOutput->currentText();
}

void Preferences::reset()
{
    ui->port->setText(_port);
    ui->audioInput->setCurrentIndex(ui->audioInput->findText(_selectedInputDevice));
    ui->audioOutput->setCurrentIndex(ui->audioOutput->findText(_selectedOutputDevice));
}
