SOURCES += \
    $$PWD/models/gpgkeylistmodel.cpp \
    $$PWD/mainwindow.cpp \
    $$PWD/preferencedialog.cpp \
    $$PWD/settings.cpp

HEADERS += \
    $$PWD/models/gpgkeylistmodel.h \
    $$PWD/mainwindow.h \
    $$PWD/preferencedialog.h \
    $$PWD/settings.h

FORMS += \
    $$PWD/mainwindow.ui \
    $$PWD/preferencedialog.ui

include($$PWD/../config/unix.pri)
include($$PWD/../config/win32.pri)

