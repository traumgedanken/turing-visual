QT        += core gui network
CONFIG    += c++11
TARGET     = turi
TEMPLATE   = app
DEFINES   += QT_DEPRECATED_WARNINGS
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES +=                 \
    main.cpp               \
    mainwindow.cpp         \
    htmltext.cpp           \
    edge.cpp               \
    node.cpp               \
    graphwidget.cpp        \
    client.cpp             \
    turicarriagepainter.cpp


HEADERS +=                 \
    mainwindow.h           \
    htmltext.h             \
    edge.h                 \
    node.h                 \
    graphwidget.h          \
    client.h               \
    turicarriagepainter.h

FORMS +=                   \
        mainwindow.ui

INCLUDEPATH += ../common
LIBS += -L ../build-common-Desktop-Debug -lcommon

