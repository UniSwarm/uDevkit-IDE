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

#include "abstractversioncontrol.h"

#include <QDebug>
#include <QDir>

AbstractVersionControl::AbstractVersionControl(QObject *parent)
    : QObject(parent)
{
}

QString AbstractVersionControl::versionControlName() const
{
    return "none";
}

void AbstractVersionControl::setPath(const QString &path)
{
    _validatedFiles.clear();
    _trackedFiles.clear();
    _modifiedFiles.clear();
    _path = QDir::cleanPath(path);
    analysePath();
}

QString AbstractVersionControl::basePath() const
{
    return _path;
}

bool AbstractVersionControl::isDirTracked(const QString &filePath) const
{
    if (!isValid())
    {
        return true;
    }
    QSetIterator<QString> i(_trackedFiles);
    while (i.hasNext())
    {
        if (i.next().startsWith(filePath))
        {
            return true;
        }
    }
    return false;
}

bool AbstractVersionControl::isFileModified(const QString &filePath) const
{
    return _modifiedFiles.contains(filePath);
}

bool AbstractVersionControl::isDirModified(const QString &filePath) const
{
    QSetIterator<QString> i(_modifiedFiles);
    while (i.hasNext())
    {
        if (i.next().startsWith(filePath))
        {
            return true;
        }
    }
    return false;
}

bool AbstractVersionControl::isFileTracked(const QString &filePath) const
{
    if (!isValid())
    {
        return true;
    }
    return _trackedFiles.contains(filePath);
}

bool AbstractVersionControl::isFileValidated(const QString &filePath) const
{
    return _validatedFiles.contains(filePath);
}

const QSet<QString> &AbstractVersionControl::trackedFiles() const
{
    return _trackedFiles;
}

const QSet<QString> &AbstractVersionControl::modifiedFiles() const
{
    return _modifiedFiles;
}

const QSet<QString> &AbstractVersionControl::validatedFiles() const
{
    return _validatedFiles;
}

void AbstractVersionControl::validFile(const QSet<QString> &filesPath)
{
    Q_UNUSED(filesPath)
}

void AbstractVersionControl::inValidFile(const QSet<QString> &filesPath)
{
    Q_UNUSED(filesPath)
}

void AbstractVersionControl::checkoutFile(const QSet<QString> &filesPath)
{
    Q_UNUSED(filesPath)
}

bool AbstractVersionControl::isValid() const
{
    return true;
}

void AbstractVersionControl::modifFile(const QSet<QString> &filesPath)
{
    QSet<QString> mnewModifiedFiles;
    for (const QString &filePath : filesPath)
    {
        if (!_modifiedFiles.contains(filePath))
        {
            mnewModifiedFiles.insert(filePath);
            _modifiedFiles.insert(filePath);
        }
    }
    if (!mnewModifiedFiles.isEmpty())
    {
        emit newModifiedFiles(mnewModifiedFiles);
    }
}

void AbstractVersionControl::requestFileModifications(const QString &filePath)
{
    Q_UNUSED(filePath)
}

FileVersionChange AbstractVersionControl::fileModifications(const QString &filePath)
{
    QMap<QString, FileVersionChange>::const_iterator localFind = _changeForFile.constFind(filePath);
    if (localFind != _changeForFile.cend())
    {
        return *localFind;
    }

    return FileVersionChange();
}

void AbstractVersionControl::analysePath()
{
}
