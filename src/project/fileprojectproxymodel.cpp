#include "fileprojectproxymodel.h"

FileProjectProxyModel::FileProjectProxyModel()
    : QSortFilterProxyModel()
{
}

bool FileProjectProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    QFileSystemModel *fsm = qobject_cast<QFileSystemModel*>(sourceModel());
    if (source_parent == fsm->index(fsm->rootPath()))
    {
        return !QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent);
    }
    return true;
}
