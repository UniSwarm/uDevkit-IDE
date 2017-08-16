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

qtHaveModule(webenginewidgets) {
    QT += webenginewidgets
    message("web engine included")
} else {
    DEFINES += NO_WEBENGINE
    message("NO web engine !")
}

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    editor/editor.cpp \
    editor/editortabwidget.cpp \
    editor/codeeditor.cpp \
    editor/hexeditor.cpp \
    editor/imageeditor.cpp \
    project/project.cpp \
    version/abstractversioncontrol.cpp \
    version/gitversioncontrol.cpp \
    project/fileprojectproxymodel.cpp \
    project/fileprojectwidget.cpp \
    project/fileprojectitemmodel.cpp \
    ui/searchreplacewidget.cpp \
    project/filetreeview.cpp \
    project/fileprojectinfo.cpp \
    make/makeparser.cpp \
    project/projectitemmodel.cpp \
    project/projectitem.cpp \
    editor/htmleditor.cpp

HEADERS += \
        mainwindow.h \
    project/fileprojectproxymodel.h \
    project/project.h \
    version/abstractversioncontrol.h \
    version/gitversioncontrol.h \
    project/fileprojectwidget.h \
    project/fileprojectitemmodel.h \
    editor/editor.h \
    editor/editors.h \
    editor/editortabwidget.h \
    editor/codeeditor.h \
    editor/hexeditor.h \
    editor/imageeditor.h \
    ui/searchreplacewidget.h \
    project/filetreeview.h \
    project/fileprojectinfo.h \
    make/makeparser.h \
    project/projectitemmodel.h \
    project/projectitem.h \
    editor/htmleditor.h

RESOURCES += $$PWD/../contrib/QDarkStyleSheet/qdarkstyle/style.qrc

include (../contrib/edbee-lib/edbee-lib/edbee-lib.pri)
include (../contrib/hexedit2.pri)
