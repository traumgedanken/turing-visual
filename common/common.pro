QT       -= gui
TARGET    = common
TEMPLATE  = lib
DEFINES  += COMMON_LIBRARY
DEFINES  += QT_DEPRECATED_WARNINGS

SOURCES  +=              \
    exceptionmessage.cpp \
    request.cpp          \
    response.cpp         \
    turicommand.cpp      \
    turiparsererror.cpp  \
    turiprogram.cpp

HEADERS  +=              \
    common_global.h      \
    defines.h            \
    exceptionmessage.h   \
    request.h            \
    response.h           \
    turicommand.h        \
    turiparsererror.h    \
    turiprogram.h


unix {
    target.path = /usr/lib
    INSTALLS += target
}
