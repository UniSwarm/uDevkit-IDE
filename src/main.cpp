/**
 ** This file is part of the uDevkit-IDE project.
 ** Copyright 2017-2021 UniSwarm
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
    QApplication::setOrganizationName("UniSwarm");
    QApplication::setOrganizationDomain("UniSwarm");
    QApplication::setApplicationName("udk-ide");

    // translate app
    QTranslator qtTranslator;
    QString lang = SettingsManager::language();
    qtTranslator.load("qt_" + lang, QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    QApplication::installTranslator(&qtTranslator);
    QTranslator udkideTranslator;
    udkideTranslator.load("udk-ide_" + lang, ":/translations");
    QApplication::installTranslator(&udkideTranslator);

    Project *project = Q_NULLPTR;
    if (argc > 1)
    {
        project = new Project(argv[1]);
    }

    MainWindow w(project);

    // apply dark style
    QFile f(":qdarkstyle/style.qss");
    if (f.exists())
    {
        f.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&f);
        app.setStyleSheet(ts.readAll());
        w.setStyleSheet("\
            QComboBox::item:checked {height: 24px;} \
            QMenu::icon {margin: 1px;} \
            QMenu::icon:checked {margin: 0; background: #505F69; border: 1px inset #505F69; position: absolute; top: 1px; right: 1px; bottom: 1px; left: 1px;} \
            QMenu::item {padding: 4px 24px 4px 8px;} \
            QToolBar {border-bottom: none;} \
            QSplitter::handle:horizontal {width: 2px;} \
            QSplitter::handle:vertical {height: 2px;} \
        ");
    }
    w.show();

    int ret = QApplication::exec();
    SettingsManager::save();
    return ret;
}
