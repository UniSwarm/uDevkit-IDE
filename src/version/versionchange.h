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

#ifndef VERSIONCHANGE_H
#define VERSIONCHANGE_H

#include <QStringList>

class VersionChange
{
public:
    VersionChange();

    int lineOld() const;
    void setLineOld(int line);

    int lineNew() const;
    void setLineNew(int line);

    QStringList &addedLines();
    const QStringList &addedLines() const;
    void addAddedLine(const QString &line);

    QStringList &removedLines();
    const QStringList &removedLines() const;
    void addRemovedLine(const QString &line);

    void clear();
    bool isValid() const;

    bool isStaged() const;
    void setStaged(bool staged);

protected:
    int _lineOld;
    int _lineNew;
    QStringList _addedLines;
    QStringList _removedLines;
    bool _staged;
};

#endif // VERSIONCHANGE_H
