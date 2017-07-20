
#include "fileprojectproxymodel.h"

#include <QFont>
#include <QDebug>

FileProjectProxyModel::FileProjectProxyModel(Project *project)
    : QSortFilterProxyModel()
{
    _project = project;
}

QVariant FileProjectProxyModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::FontRole)
    {
        if (!_project || !index.isValid() || !sourceModel())
            return QSortFilterProxyModel::data(index, role);
        QFileSystemModel *fsm = qobject_cast<QFileSystemModel*>(sourceModel());
        if (_project->versionControl()->isFileModified(fsm->filePath(mapToSource(index))))
        {
            QFont font;
            font.setBold(true);
            return font;
        }
        return QVariant();
    }
    return QSortFilterProxyModel::data(index, role);
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
