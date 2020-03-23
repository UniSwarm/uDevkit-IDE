/**
 ** This file is part of the uDevkit-IDE project.
 ** Copyright 2017-2020 UniSwarm
 **
 ** This program is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU General Public License as published by
 ** the Free Software Foundation, either version 3 of the License, or
 ** (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 ** GNU General Public License for more details.
 **
 ** You should have received a copy of the GNU General Public License
 ** along with this program. If not, see <http://www.gnu.org/licenses/>.
 **/

#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <QLibraryInfo>
#include <QTranslator>

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
    QTranslator udkideTranslator;
    udkideTranslator.load("udk-ide_" + lang, ":/translations");
    app.installTranslator(&udkideTranslator);

    Project *project = Q_NULLPTR;
    if (argc > 1)
        project = new Project(argv[1]);

    MainWindow w(project);
    w.show();

    int ret = app.exec();
    SettingsManager::save();
    return ret;
}
