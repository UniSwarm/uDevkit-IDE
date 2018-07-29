#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <QTranslator>
#include <QLibraryInfo>

#include "project/project.h"
#include "settings/settingsmanager.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setOrganizationName("UniSwarm");
    app.setOrganizationDomain("UniSwarm");
    app.setApplicationName("udk-ide");

    // apply dark style
    QFile f(":qdarkstyle/style.qss");
    if (f.exists())
    {
        f.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&f);
        qApp->setStyleSheet(ts.readAll());
    }

    // translate app
    QTranslator qtTranslator;
    QString lang = SettingsManager::language();
    qtTranslator.load("qt_" + lang, QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    app.installTranslator(&qtTranslator);
    QTranslator rtideTranslator;
    rtideTranslator.load("rtide_" + lang, ":/translations");
    app.installTranslator(&rtideTranslator);

    Project *project = Q_NULLPTR;
    if (argc > 1)
        project = new Project(argv[1]);
    else
        project = new Project(QDir().absolutePath());

    MainWindow w(project);
    w.show();

    int ret = app.exec();
    SettingsManager::save();
    return ret;
}
