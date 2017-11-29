#-------------------------------------------------
#
# Project created by QtCreator 2017-07-04T17:45:01
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = rtide
TEMPLATE = app
DESTDIR = $$PWD/../bin

CONFIG += c++11

# Qt web kit / web engine / HTMLDoc selection
qtHaveModule(webenginewidgets) {
    QT += webenginewidgets
    DEFINES += WEBENGINE
    message("web engine included")
} else {
    qtHaveModule(webkitwidgets) {
        QT += webkitwidgets
        DEFINES += WEBKIT
        message("web kit included")
    } else {
        DEFINES += NOWEBKIT
        message("NO web engine !")
    }
}

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    editor/editor.cpp \
    editor/editortabwidget.cpp \
    editor/codeeditor/codeeditor.cpp \
    editor/codeeditor/codeeditorscrollbar.cpp \
    editor/codeeditor/codeeditormargindelegate.cpp \
    editor/codeeditor/htmleditor.cpp \
    editor/hexeditor.cpp \
    editor/imageeditor.cpp \
    version/abstractversioncontrol.cpp \
    version/gitversioncontrol.cpp \
    ui/searchreplacewidget.cpp \
    make/makeparser.cpp \
    project/fileprojectinfo.cpp \
    project/project.cpp \
    project/projectitemmodel.cpp \
    project/projectitem.cpp \
    project/projectitemproxymodel.cpp \
    project/projecttreeview.cpp \
    project/projectwidget.cpp \
    ui/logwidget.cpp \
    version/versionchange.cpp \
    version/fileversionchange.cpp \
    project/projecticonprovider.cpp \
    project/fileproject.cpp

HEADERS += \
    mainwindow.h \
    version/abstractversioncontrol.h \
    version/gitversioncontrol.h \
    editor/editor.h \
    editor/editors.h \
    editor/editortabwidget.h \
    editor/codeeditor/codeeditor.h \
    editor/codeeditor/codeeditorscrollbar.h \
    editor/codeeditor/codeeditormargindelegate.h \
    editor/codeeditor/htmleditor.h \
    editor/hexeditor.h \
    editor/imageeditor.h \
    ui/searchreplacewidget.h \
    make/makeparser.h \
    project/fileprojectinfo.h \
    project/project.h \
    project/projectitemmodel.h \
    project/projectitem.h \
    project/projectitemproxymodel.h \
    project/projecttreeview.h \
    project/projectwidget.h \
    ui/logwidget.h \
    version/versionchange.h \
    version/fileversionchange.h \
    project/projecticonprovider.h \
    project/fileproject.h

RESOURCES += $$PWD/../contrib/QDarkStyleSheet/qdarkstyle/style.qrc

RESOURCES += \
        $$PWD/img.qrc

win32 : RC_FILE = rtide.rc

include (../contrib/edbee-lib/edbee-lib/edbee-lib.pri)
include (../contrib/hexedit2.pri)
