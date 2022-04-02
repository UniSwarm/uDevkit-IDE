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

#ifndef PROJECT_H
#define PROJECT_H

#include <QDir>
#include <QObject>

#include "make/makeparser.h"
#include "version/abstractversioncontrol.h"

#include "projectitemmodel.h"

class Project : public QObject
{
    Q_OBJECT
public:
    explicit Project(const QString &path = QString());
    ~Project() override;

    const QDir &rootDir() const;
    QString rootPath() const;
    void setRootPath(const QString &path);

    AbstractVersionControl *versionControl() const;

    ProjectItemModel *projectItemModel() const;

    bool isOpenedFile(const QString &path) const;
    const QSet<QString> &openedFiles() const;
    void addOpenedFiles(const QSet<QString> &openedFiles);
    void removeOpenedFiles(const QSet<QString> &openedFiles);

    MakeParser *make() const;

signals:
    void rootPathChanged();

public slots:

protected slots:
    void newSource(const QSet<QString> &sources);
    void oldSource(const QSet<QString> &sources);

protected:
    QDir _rootDir;
    AbstractVersionControl *_versionControl;
    QSet<QString> _openedFiles;
    MakeParser *_make;

    ProjectItemModel *_projectItemModel;
};

#endif // PROJECT_H
