#-------------------------------------------------
#
# Project created by QtCreator 2017-07-04T17:45:01
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = rtide
TEMPLATE = app
DESTDIR = $$PWD/../bin

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    project/fileproject.cpp \
    editor/codeeditor.cpp

HEADERS += \
        mainwindow.h \
    project/fileproject.h \
    editor/codeeditor.h \
    project/fileprojectproxymodel.h

include (../edbee-lib/edbee-lib/edbee-lib.pri)
