CONFIG += qt

QT += core gui widgets

TARGET = QGPG
TEMPLATE = app

SOURCES += main.cpp

include(../src/link.pri)
include(../config/unix.pri)
include(../config/win32.pri)
