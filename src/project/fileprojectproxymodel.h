#ifndef FILEPROJECTPROXYMODEL_H
#define FILEPROJECTPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QFileSystemModel>
#include <QDebug>

#include "project.h"

class FileProjectProxyModel : public QSortFilterProxyModel
{
public:
    explicit FileProjectProxyModel(Project *project);

protected:
    virtual bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;

    // QAbstractItemModel interface
public:
    virtual QVariant data(const QModelIndex &index, int role) const;

protected:
    Project *_project;
};

#endif // FILEPROJECTPROXYMODEL_H
