#-------------------------------------------------
#
# Project created by QtCreator 2017-07-04T17:45:01
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = udk-ide
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
    $$PWD/editor/codeeditor/codeeditor.cpp \
    $$PWD/editor/codeeditor/codeeditormargindelegate.cpp \
    $$PWD/editor/codeeditor/codeeditorscrollbar.cpp \
    $$PWD/editor/codeeditor/htmleditor.cpp \
    $$PWD/editor/codeeditor/markdowneditor.cpp \
    $$PWD/editor/editor.cpp \
    $$PWD/editor/editortabswitchwidget.cpp \
    $$PWD/editor/editortabwidget.cpp \
    $$PWD/editor/hexeditor.cpp \
    $$PWD/editor/hexfile.cpp \
    $$PWD/editor/hexfileeditor.cpp \
    $$PWD/editor/imageeditor.cpp \
    $$PWD/main.cpp \
    $$PWD/mainwindow.cpp \
    $$PWD/make/makeparser.cpp \
    $$PWD/make/makerule.cpp \
    $$PWD/project/fileproject.cpp \
    $$PWD/project/fileprojectinfo.cpp \
    $$PWD/project/project.cpp \
    $$PWD/project/projecticonprovider.cpp \
    $$PWD/project/projectitem.cpp \
    $$PWD/project/projectitemmodel.cpp \
    $$PWD/project/projectitemproxymodel.cpp \
    $$PWD/project/projecttreeview.cpp \
    $$PWD/project/projecttreeviewitemdelegate.cpp \
    $$PWD/project/projectwidget.cpp \
    $$PWD/settings/pages/codeeditorsettings.cpp \
    $$PWD/settings/pages/gitversionsettings.cpp \
    $$PWD/settings/pages/globalsettings.cpp \
    $$PWD/settings/pages/hexeditorsettings.cpp \
    $$PWD/settings/pages/htmleditorsettings.cpp \
    $$PWD/settings/pages/imageeditorsettings.cpp \
    $$PWD/settings/pages/settingscateg.cpp \
    $$PWD/settings/pages/settingspage.cpp \
    $$PWD/settings/pages/settingswindow.cpp \
    $$PWD/settings/pages/toolsversionsettings.cpp \
    $$PWD/settings/setting.cpp \
    $$PWD/settings/settingsclass.cpp \
    $$PWD/settings/settingsmanager.cpp \
    $$PWD/settings/widgets/patheditwidget.cpp \
    $$PWD/ui/iconneddockstyle.cpp \
    $$PWD/ui/logwidget.cpp \
    $$PWD/ui/searchreplacewidget.cpp \
    $$PWD/version/abstractversioncontrol.cpp \
    $$PWD/version/fileversionchange.cpp \
    $$PWD/version/gitversioncontrol.cpp \
    $$PWD/version/versionchange.cpp

HEADERS += \
    $$PWD/editor/codeeditor/codeeditor.h \
    $$PWD/editor/codeeditor/codeeditormargindelegate.h \
    $$PWD/editor/codeeditor/codeeditorscrollbar.h \
    $$PWD/editor/codeeditor/htmleditor.h \
    $$PWD/editor/codeeditor/markdowneditor.h \
    $$PWD/editor/editor.h \
    $$PWD/editor/editors.h \
    $$PWD/editor/editortabswitchwidget.h \
    $$PWD/editor/editortabwidget.h \
    $$PWD/editor/hexeditor.h \
    $$PWD/editor/hexfileeditor.h \
    $$PWD/editor/hexfile.h \
    $$PWD/editor/imageeditor.h \
    $$PWD/mainwindow.h \
    $$PWD/make/makeparser.h \
    $$PWD/make/makerule.h \
    $$PWD/project/fileproject.h \
    $$PWD/project/fileprojectinfo.h \
    $$PWD/project/project.h \
    $$PWD/project/projecticonprovider.h \
    $$PWD/project/projectitem.h \
    $$PWD/project/projectitemmodel.h \
    $$PWD/project/projectitemproxymodel.h \
    $$PWD/project/projecttreeview.h \
    $$PWD/project/projecttreeviewitemdelegate.h \
    $$PWD/project/projectwidget.h \
    $$PWD/settings/pages/codeeditorsettings.h \
    $$PWD/settings/pages/gitversionsettings.h \
    $$PWD/settings/pages/globalsettings.h \
    $$PWD/settings/pages/hexeditorsettings.h \
    $$PWD/settings/pages/htmleditorsettings.h \
    $$PWD/settings/pages/imageeditorsettings.h \
    $$PWD/settings/pages/settingscateg.h \
    $$PWD/settings/pages/settingspage.h \
    $$PWD/settings/pages/settingswindow.h \
    $$PWD/settings/pages/toolsversionsettings.h \
    $$PWD/settings/setting.h \
    $$PWD/settings/settingsclass.h \
    $$PWD/settings/settingsmanager.h \
    $$PWD/settings/widgets/patheditwidget.h \
    $$PWD/ui/iconneddockstyle.h \
    $$PWD/ui/logwidget.h \
    $$PWD/ui/searchreplacewidget.h \
    $$PWD/version/abstractversioncontrol.h \
    $$PWD/version/fileversionchange.h \
    $$PWD/version/gitversioncontrol.h \
    $$PWD/version/versionchange.h

RESOURCES += $$PWD/../contrib/QDarkStyleSheet/qdarkstyle/style.qrc

RESOURCES += $$PWD/img.qrc

win32|win64
{
    RC_FILE = udk-ide.rc
}

isEmpty(PREFIX)
{
    PREFIX=/usr/local
}
target.path=$$PREFIX/bin
unix
{
    QMAKE_SUBSTITUTES += udk-ide.desktop.in
    desktop.path  = $$PREFIX/share/applications
    desktop.files = udk-ide.desktop
    INSTALLS += desktop
    icons.path  = $$PREFIX/share/icons
    icons.files = udk-ide.ico
    INSTALLS += icons
    data.path  = $$PREFIX/data
    data.files = ../data/*
    INSTALLS += data
    syntaxes.path  = $$PREFIX/data/syntaxfiles
    syntaxes.files = ../contrib/edbee-data/syntaxfiles/*
    INSTALLS += syntaxes
}
!isEmpty(target.path): INSTALLS += target

include (../contrib/edbee-lib/edbee-lib/edbee-lib.pri)
include (../contrib/QHexView/QHexView.pri)
include (../contrib/maddy/maddy.pri)

TRANSLATIONS = $$PWD/translations/udk-ide_fr.ts \
               $$PWD/translations/udk-ide_en.ts
qtPrepareTool(LRELEASE, lrelease)
for(tsfile, TRANSLATIONS) {
    command = $$LRELEASE $$tsfile
    system($$command)|error("Failed to run: $$command")
}
RESOURCES += $$PWD/translations/translations.qrc
