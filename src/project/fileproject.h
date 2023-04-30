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

#ifndef FILEPROJECT_H
#define FILEPROJECT_H

#include <QObject>

#include "fileprojectinfo.h"
#include "versioncontrol/abstractversioncontrol.h"

class Editor;

class FileProject : public QObject
{
    Q_OBJECT
public:
    FileProject(const QString &filePath, Project *project);

    const FileProjectInfo &info() const;

    QList<Editor *> openedEditors();

    // version control interface
    void valid(int line = -1);
    void checkout(int line = -1);
    void inValid(int line = -1);
    void updateDiff();

signals:
    void fileContentChanged();
    void versionStatusChanged();
    void versionDiffChanged();

protected:
private:
    Q_DISABLE_COPY(FileProject)
    FileProjectInfo _info;
    Project *_project;

    AbstractVersionControl *_versionControl;
    FileVersionChange *_fileChange;
};

#endif  // FILEPROJECT_H
