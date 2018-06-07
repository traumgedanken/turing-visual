#-------------------------------------------------
#
# Project created by QtCreator 2018-06-07T09:55:28
#
#-------------------------------------------------

QT       -= gui

TARGET = common
TEMPLATE = lib

DEFINES += COMMON_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        exceptionmessage.cpp \
        request.cpp \
        response.cpp \
        turicommand.cpp \
        turiparsererror.cpp \
        turiprogram.cpp

HEADERS += \
        common_global.h \
        defines.h \
        exceptionmessage.h \
        request.h \
        response.h \
        turicommand.h \
        turiparsererror.h \
        turiprogram.h


unix {
    target.path = /usr/lib
    INSTALLS += target
}
