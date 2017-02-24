CONFIG += qt

QT += core gui widgets

TARGET = QGPG
TEMPLATE = app

SOURCES += main.cpp

include(../src/src.pri)
include(../config/unix.pri)
include(../config/win32.pri)
win32 {
    include($$PWD/windows/version.pri)
}
