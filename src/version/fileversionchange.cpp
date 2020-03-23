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

#include "fileversionchange.h"

#include <QDebug>

FileVersionChange::FileVersionChange()
{
}

FileVersionChange::FileVersionChange(const FileVersionChange &other)
{
    foreach (VersionChange *change, other._changes)
    {
        _changes.append(new VersionChange(*change));
    }
}

FileVersionChange::~FileVersionChange()
{
    qDeleteAll(_changes);
}

FileVersionChange &FileVersionChange::operator=(const FileVersionChange &other)
{
    if (this != &other)
    {
        qDeleteAll(_changes);
        _changes.clear();
        foreach (VersionChange *change, other._changes)
        {
            _changes.append(new VersionChange(*change));
        }
    }
    return *this;
}

QList<VersionChange *> &FileVersionChange::changes()
{
    return _changes;
}

QList<VersionChange *> FileVersionChange::changesForRange(int lineStart, int lineEnd) const
{
    QList<VersionChange *> changes;
    foreach (VersionChange *change, _changes)
    {
        if (change->lineNew() <= lineEnd && (change->lineNew() + change->addedLines().count()) > lineStart)
        {
            changes.append(change);
        }
        else if (change->addedLines().isEmpty() && change->lineNew() == lineStart)
        {
            changes.append(change);
        }
    }
    return changes;
}

void FileVersionChange::insertChange(const VersionChange &change)
{
    // update all change after the current insert pos
    int lineOffset = change.addedLines().count() - change.removedLines().count();
    insertLines(change.lineNew(), lineOffset);

    QList<VersionChange *> actualChanges = changesForRange(change.lineNew(), change.lineNew());
    VersionChange *nchange;
    if (actualChanges.isEmpty()) // add a new change here
    {
        nchange = new VersionChange(change);
        _changes.append(nchange);
    }
    else // merge with already existing changes
    {
        nchange = actualChanges.first();
        /*for (int i=nchange->lineNew(); i<nchange->addedLines().count()-1; i++)
            nchange->addedLines().addAddedLine(nchange->addedLines().at(i));*/
        nchange->addedLines() = change.addedLines();
        // TODO complete merge
    }
    // qDebug()<<nchange->removedLines().count()<<nchange->addedLines().count();
    // qDebug()<<nchange->removedLines()<<nchange->addedLines();
}

void FileVersionChange::clear()
{
    _changes.clear();
}

void FileVersionChange::insertLines(int fromLine, int lineCount)
{
    foreach (VersionChange *change, _changes)
    {
        if (change->lineNew() > fromLine)
        {
            change->setLineNew(change->lineNew() + lineCount);
        }
    }
}
