SOURCES += \
    $$PWD/models/gpgkeylistmodel.cpp \
    $$PWD/mainwindow.cpp

HEADERS += \
    $$PWD/models/gpgkeylistmodel.h \
    $$PWD/mainwindow.h

FORMS += \
    $$PWD/mainwindow.ui

include($$PWD/../config/unix.pri)
include($$PWD/../config/win32.pri)

