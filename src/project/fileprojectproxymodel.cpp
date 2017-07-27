#include "fileprojectproxymodel.h"

#include <QFileSystemModel>

FileProjectProxyModel::FileProjectProxyModel(Project *project)
    : QSortFilterProxyModel()
{
    _project = project;
    setDynamicSortFilter(true);
}

void FileProjectProxyModel::setHiddenFilter(const QRegExp &regExp)
{
    _hiddenFilter = regExp;
}

void FileProjectProxyModel::setShowFilter(const QRegExp &regExp)
{
    _showFilter = regExp;
}

bool FileProjectProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    QFileSystemModel *fsm = static_cast<QFileSystemModel*>(sourceModel());
    const QModelIndex index = fsm->index(source_row, 0, source_parent);
    const QString &path = fsm->data(index, QFileSystemModel::FilePathRole).toString();

    // hidden filter: if path match, do not show
    if (_hiddenFilter.indexIn(path) != -1)
        return false;

    // show filter: show only if match
    if (fsm->isDir(index))
        return true;
    if (_showFilter.indexIn(path) == -1)
        return false;

    return true;
}

bool FileProjectProxyModel::filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const
{
    Q_UNUSED(source_column)
    Q_UNUSED(source_parent)
    return true;
}
