#include "settings.h"
#include <QCoreApplication>

Settings::Settings()
{
    QCoreApplication::setOrganizationName("FH-SWF");
    QCoreApplication::setOrganizationDomain("fh-swf.de");
    QCoreApplication::setApplicationName("Encrypted Voice Chat");
}
