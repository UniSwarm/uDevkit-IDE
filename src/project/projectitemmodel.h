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

#ifndef PROJECTMODEL_H
#define PROJECTMODEL_H

#include <QAbstractItemModel>
#include <QFileIconProvider>

#include "projectitem.h"

class ProjectItemModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    ProjectItemModel(Project *project);
    ~ProjectItemModel() override;

    void addFileItem(const QString &path);
    void addRealDirItem(const QString &path);
    void addLogicDirItem(const QString &name);
    void addItem(ProjectItem *item);

    void clear();

    enum Role
    {
        FileIconRole = Qt::DecorationRole,
        FilePathRole = Qt::UserRole + 1,
        FileNameRole = Qt::UserRole + 2
    };
    const ProjectItem *item(const QModelIndex &index) const;
    // QModelIndex index(const ProjectItem *item) const;
    QModelIndex index(const QString path) const;
    bool isDir(const QModelIndex &index) const;
    bool isFile(const QModelIndex &index) const;
    QString filePath(const QModelIndex &index) const;
    QString fileName(const QModelIndex &index) const;
    bool rmdir(const QModelIndex &index);
    bool remove(const QModelIndex &index);

    // high level interface
    void addExternalSource(QSet<QString> sourceFiles);
    void removeExternalSource(QSet<QString> sourceFiles);

    void addOtherSource(QSet<QString> sourceFiles);
    void removeOtherSource(QSet<QString> sourceFiles);

    void prepareModif();
    void endModif();

public slots:
    void filesUpdated(QSet<QString> filesPath);

    // QAbstractItemModel interface
public:
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    /*virtual bool hasChildren(const QModelIndex &parent) const;
    virtual void fetchMore(const QModelIndex &parent);
    virtual bool canFetchMore(const QModelIndex &parent) const;*/

protected:
    ProjectItem *_root;
    Project *_project;
    QFileIconProvider *_iconProvider;
    QMap<QString, ProjectItem *> _pathCache;

    // high level interface
    ProjectItem *_externalFiles;
    ProjectItem *_otherFiles;
};

#endif // PROJECTMODEL_H
