#include "mainwindow.h"
#include <QApplication>
#include <QDebug>

#include "project/project.h"
#include "settings/settingsmanager.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName("UniSwarm");
    a.setOrganizationDomain("UniSwarm");
    a.setApplicationName("RtIDE");

    // apply dark style
    QFile f(":qdarkstyle/style.qss");
    if (f.exists())
    {
        f.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&f);
        qApp->setStyleSheet(ts.readAll());
    }

    Project *project = nullptr;
    if (argc > 1)
        project = new Project(argv[1]);
    else
        project = new Project(QDir().absolutePath());

    MainWindow w(project);
    w.show();

    int ret = a.exec();
    SettingsManager::save();
    return ret;
}
