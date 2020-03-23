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

#include "versionchange.h"

VersionChange::VersionChange()
{
    _staged = false;
}

int VersionChange::lineOld() const
{
    return _lineOld;
}

void VersionChange::setLineOld(int line)
{
    _lineOld = line;
}

int VersionChange::lineNew() const
{
    return _lineNew;
}

void VersionChange::setLineNew(int line)
{
    _lineNew = line;
}

QStringList &VersionChange::addedLines()
{
    return _addedLines;
}

const QStringList &VersionChange::addedLines() const
{
    return _addedLines;
}

void VersionChange::addAddedLine(const QString &line)
{
    _addedLines.append(line);
}

QStringList &VersionChange::removedLines()
{
    return _removedLines;
}

const QStringList &VersionChange::removedLines() const
{
    return _removedLines;
}

void VersionChange::addRemovedLine(const QString &line)
{
    _removedLines.append(line);
}

void VersionChange::clear()
{
    _lineOld = -1;
    _lineNew = -1;
    _addedLines.clear();
    _removedLines.clear();
}

bool VersionChange::isValid() const
{
    if (_lineOld == -1 || _lineNew == -1)
    {
        return false;
    }
    if (_addedLines.isEmpty() && _removedLines.isEmpty())
    {
        return false;
    }
    return true;
}

bool VersionChange::isStaged() const
{
    return _staged;
}

void VersionChange::setStaged(bool staged)
{
    _staged = staged;
}
