#-------------------------------------------------
#
# Project created by QtCreator 2011-02-15T02:32:21
#
#-------------------------------------------------

QT       += core gui multimedia network

TARGET = QtEncryptedVoiceChatClient
TEMPLATE = app


SOURCES += main.cpp\
           mainwindow.cpp \
           audioinput.cpp \
           audiooutput.cpp \
           audio.cpp \
           client.cpp \
           server.cpp \
           choice.cpp \
    servermessages.cpp \
    login.cpp \
    serverconnection.cpp \
    clientconnection.cpp \
    user.cpp

HEADERS  += mainwindow.h \
            audioinput.h \
            audiooutput.h \
            audio.h \
            client.h \
            server.h \
            choice.h \
    servermessages.h \
    login.h \
    serverconnection.h \
    clientconnection.h \
    user.h

FORMS    += mainwindow.ui \
            client.ui \
            server.ui \
            choice.ui \
    login.ui

OTHER_FILES += README.markdown
