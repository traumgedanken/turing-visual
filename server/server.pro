QT       -= gui
QT       += core network
CONFIG   += c++11 console
CONFIG   -= app_bundle
DEFINES  += QT_DEPRECATED_WARNINGS
TEMPLATE  = app

SOURCES  +=         \
    main.cpp        \
    server.cpp      \
    turiparser.cpp  \
    turicarriage.cpp

HEADERS  +=         \
    server.h        \
    turiparser.h    \
    turicarriage.h

INCLUDEPATH += ../common
LIBS += -L ../build-common-Desktop-Debug -lcommon
