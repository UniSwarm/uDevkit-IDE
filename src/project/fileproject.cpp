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

#include "fileproject.h"

FileProject::FileProject(const QString &filePath, Project *project)
    : _info(filePath, project)
    , _project(project)
{
    _versionControl = Q_NULLPTR;
    _fileChange = Q_NULLPTR;
}

const FileProjectInfo &FileProject::info() const
{
    return _info;
}

QList<Editor *> FileProject::openedEditors()
{
    // TODO implement me
    return QList<Editor *>();
}

void FileProject::valid(int line)
{
    Q_UNUSED(line)
    if (!_versionControl)
    {
        return;
    }
    _versionControl->validFile(QSet<QString>() << _info.filePath());
}

void FileProject::checkout(int line)
{
    Q_UNUSED(line)
    if (!_versionControl)
    {
        return;
    }
    _versionControl->checkoutFile(QSet<QString>() << _info.filePath());
}

void FileProject::inValid(int line)
{
    Q_UNUSED(line)
    if (!_versionControl)
    {
        return;
    }
    _versionControl->inValidFile(QSet<QString>() << _info.filePath());
}

void FileProject::updateDiff()
{
    if (!_versionControl)
    {
        return;
    }
    _versionControl->requestFileModifications(_info.filePath());
}
