CONFIG += qt

QT += core gui widgets

TARGET = src
TEMPLATE = lib
CONFIG += staticlib

include(src.pri)

OTHER_FILES += \
    $$PWD/QGPG_ja.ts
