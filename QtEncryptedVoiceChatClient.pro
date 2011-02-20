#-------------------------------------------------
#
# Project created by QtCreator 2011-02-15T02:32:21
#
#-------------------------------------------------

QT       += core gui multimedia

TARGET = QtEncryptedVoiceChatClient
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    audioinput.cpp \
    audiooutput.cpp \
    audio.cpp

HEADERS  += mainwindow.h \
    audioinput.h \
    audiooutput.h \
    audio.h

FORMS    += mainwindow.ui

OTHER_FILES += \
    README.markdown
