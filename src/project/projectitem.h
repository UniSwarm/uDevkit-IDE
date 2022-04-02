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

#ifndef PROJECTITEM_H
#define PROJECTITEM_H

#include <QFileSystemWatcher>
#include <QHash>
#include <QList>
#include <QObject>

class Project;
class ProjectItemModel;

#include "fileprojectinfo.h"

class ProjectItem : public QObject
{
    Q_OBJECT
public:
    enum Type
    {
        RealDir,
        DirFile,
        LogicDir,
        IndividualFile
    };

    explicit ProjectItem(Project *project, const QString &path, Type type, ProjectItemModel *model);
    ~ProjectItem() override;

    int count() const;
    ProjectItem *child(int row) const;
    ProjectItem *child(const QString &name) const;
    int row() const;
    ProjectItem *parentItem() const;
    void addChild(ProjectItem *child);
    void removeChild(ProjectItem *child);
    void remove();
    void clear();
    Type type() const;

    void addFileItem(const QString &path);
    void addRealDirItem(const QString &path);
    void addLogicDirItem(const QString &name);

    QVariant data(int column, int role) const;

    const FileProjectInfo &info() const;
    inline QString filePath() const
    {
        return _info.filePath();
    }
    inline QString fileName() const
    {
        return _info.fileName();
    }

    /*bool haveChildren() const;
    bool canFetchMore() const;
    void fetchMore();*/

signals:

public slots:

protected slots:
    void updateModif(const QString &path = QString());

protected:
    Type _type;
    FileProjectInfo _info;
    QList<ProjectItem *> _childrens;
    QHash<QString, ProjectItem *> _childrensMap;
    ProjectItem *_parentItem;

    QFileSystemWatcher *_watcher;
    ProjectItemModel *_model;
    bool _loaded;
};

#endif  // PROJECTITEM_H
