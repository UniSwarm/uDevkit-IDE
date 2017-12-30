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
    editor/editortabswitchwidget.cpp \
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
    project/fileproject.cpp \
    ui/iconneddockstyle.cpp \
    settings/setting.cpp \
    settings/settingsmanager.cpp \
    settings/settingsclass.cpp \
    settings/widgets/patheditwidget.cpp \
    settings/pages/settingswindow.cpp \
    settings/pages/settingspage.cpp \
    settings/pages/settingscateg.cpp \
    settings/pages/codeeditorsettings.cpp \
    settings/pages/hexeditorsettings.cpp \
    settings/pages/imageeditorsettings.cpp \
    settings/pages/htmleditorsettings.cpp \
    settings/pages/gitversionsettings.cpp \
    settings/pages/globalsettings.cpp \
    settings/pages/toolsversionsettings.cpp \
    project/projecttreeviewitemdelegate.cpp

HEADERS += \
    mainwindow.h \
    version/abstractversioncontrol.h \
    version/gitversioncontrol.h \
    editor/editor.h \
    editor/editors.h \
    editor/editortabwidget.h \
    editor/editortabswitchwidget.h \
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
    project/fileproject.h \
    ui/iconneddockstyle.h \
    settings/setting.h \
    settings/settingsmanager.h \
    settings/settingsclass.h \
    settings/widgets/patheditwidget.h \
    settings/pages/settingswindow.h \
    settings/pages/settingspage.h \
    settings/pages/settingscateg.h \
    settings/pages/codeeditorsettings.h \
    settings/pages/hexeditorsettings.h \
    settings/pages/imageeditorsettings.h \
    settings/pages/htmleditorsettings.h \
    settings/pages/gitversionsettings.h \
    settings/pages/globalsettings.h \
    settings/pages/toolsversionsettings.h \
    project/projecttreeviewitemdelegate.h

RESOURCES += $$PWD/../contrib/QDarkStyleSheet/qdarkstyle/style.qrc

RESOURCES += $$PWD/img.qrc

win32 : RC_FILE = rtide.rc

include (../contrib/edbee-lib/edbee-lib/edbee-lib.pri)
include (../contrib/hexedit2.pri)

TRANSLATIONS = $$PWD/translations/rtide_fr.ts \
               $$PWD/translations/rtide_en.ts
qtPrepareTool(LRELEASE, lrelease)
for(tsfile, TRANSLATIONS) {
    command = $$LRELEASE $$tsfile
    system($$command)|error("Failed to run: $$command")
}
RESOURCES += $$PWD/translations/translations.qrc
