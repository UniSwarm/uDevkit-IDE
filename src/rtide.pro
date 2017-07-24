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

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    editor/codeeditor.cpp \
    project/project.cpp \
    version/abstractversioncontrol.cpp \
    version/gitversioncontrol.cpp \
    project/fileprojectproxymodel.cpp \
    project/fileprojectwidget.cpp \
    project/fileprojectitemmodel.cpp

HEADERS += \
        mainwindow.h \
    editor/codeeditor.h \
    project/fileprojectproxymodel.h \
    project/project.h \
    version/abstractversioncontrol.h \
    version/gitversioncontrol.h \
    project/fileprojectwidget.h \
    project/fileprojectitemmodel.h

RESOURCES += $$PWD/../contrib/QDarkStyleSheet/qdarkstyle/style.qrc

include (../contrib/edbee-lib/edbee-lib/edbee-lib.pri)
