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

#include "fileprojectinfo.h"

#include "project.h"

#include <QCoreApplication>
#include <QDateTime>

FileProjectInfo::FileProjectInfo(const QString &filePath, Project *project)
    : QFileInfo(filePath)
    , _project(project)
{
}

bool FileProjectInfo::isTracked() const
{
    if (isDir())
    {
        return _project->versionControl()->isDirTracked(filePath());
    }
    else
    {
        return _project->versionControl()->isFileTracked(filePath());
    }
}

bool FileProjectInfo::isModified() const
{
    if (isDir())
    {
        return _project->versionControl()->isDirModified(filePath());
    }
    else
    {
        return _project->versionControl()->isFileModified(filePath());
    }
}

bool FileProjectInfo::isValidated() const
{
    return _project->versionControl()->isFileValidated(filePath());
}

bool FileProjectInfo::isOpened() const
{
    return _project->isOpenedFile(filePath());
}

bool FileProjectInfo::isInProject() const
{
    return filePath().startsWith(_project->rootPath());
}

QString FileProjectInfo::humanSize() const
{
    double num = size();
    QStringList list;
    list << QCoreApplication::translate("FileProjectInfo", "kB") << QCoreApplication::translate("FileProjectInfo", "MB") << QCoreApplication::translate("FileProjectInfo", "GB")
         << QCoreApplication::translate("FileProjectInfo", "TB");

    QStringListIterator i(list);
    QString unit(QCoreApplication::translate("FileProjectInfo", "bytes"));

    while (num >= 1024.0 && i.hasNext())
    {
        unit = i.next();
        num /= 1024.0;
    }
    return QString().setNum(num, 'f', 0) + " " + unit;
}

QString FileProjectInfo::toolTips() const
{
    QString toolTip;
    toolTip.append(QCoreApplication::translate("FileProjectInfo", "Size: "));
    toolTip.append(humanSize());
    toolTip.append("<br/>");

    toolTip.append(QCoreApplication::translate("FileProjectInfo", "Modified: "));
    toolTip.append(lastModified().toString("yyyy-MM-dd hh:mm:ss"));
    toolTip.append("<br/>");
    return toolTip;
}

Project *FileProjectInfo::project() const
{
    return _project;
}
