#ifndef FILEPROJECTPROXYMODEL_H
#define FILEPROJECTPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QFileSystemModel>

class FileProjectProxyModel : public QSortFilterProxyModel
{
protected:
    virtual bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
    {
        QFileSystemModel *fsm = qobject_cast<QFileSystemModel*>(sourceModel());
        if (source_parent == fsm->index(fsm->rootPath()))
        {
            return !QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent);
        }
        return true;
    }
};

#endif // FILEPROJECTPROXYMODEL_H
