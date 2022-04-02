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

#ifndef FILEPROJECTINFO_H
#define FILEPROJECTINFO_H

#include <QFileInfo>
#include <QString>

class Project;

class FileProjectInfo : public QFileInfo
{
public:
    FileProjectInfo(const QString &filePath, Project *project);

    bool isTracked() const;
    bool isModified() const;
    bool isValidated() const;
    bool isOpened() const;
    bool isInProject() const;

    QString humanSize() const;
    QString toolTips() const;

    Project *project() const;

protected:
    Project *_project;
};

#endif  // FILEPROJECTINFO_H
