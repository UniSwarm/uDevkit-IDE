#include "fileprojectproxymodel.h"

#include <QDebug>
#include <QFileSystemModel>

#include "project.h"

FileProjectProxyModel::FileProjectProxyModel(Project *project)
    : QSortFilterProxyModel()
{
    _project = project;
    setDynamicSortFilter(true);
    setSortRole(Qt::DisplayRole);
    sort(1, Qt::AscendingOrder);
}

void FileProjectProxyModel::setHiddenFilter(const QRegExp &regExp)
{
    _hiddenFilter = regExp;
    invalidateFilter();
}

void FileProjectProxyModel::setHiddenFilter(const QString &pattern)
{
    _hiddenFilter = QRegExp(pattern, Qt::CaseInsensitive, QRegExp::RegExp);
    invalidateFilter();
}

void FileProjectProxyModel::setShowFilter(const QRegExp &regExp)
{
    _showFilter = regExp;
    invalidateFilter();
}

void FileProjectProxyModel::setShowFilter(const QString &pattern)
{
    _showFilter = QRegExp(pattern, Qt::CaseInsensitive, QRegExp::RegExp);
    invalidateFilter();
}

bool FileProjectProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    QFileSystemModel *fsm = static_cast<QFileSystemModel*>(sourceModel());
    const QModelIndex index = fsm->index(source_row, 0, source_parent);
    const QString &path = fsm->data(index, QFileSystemModel::FilePathRole).toString();

    // show all dir parent of project path
    if (!path.startsWith(_project->rootPath()))
        return true;

    // hidden filter: if path match, do not show
    if (_hiddenFilter.indexIn(path) != -1)
        return false;

    // show filter: show only if match
    if (fsm->isDir(index))
        return true; // TODO add recursive search
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

bool FileProjectProxyModel::lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const
{
    QFileSystemModel *fsm = static_cast<QFileSystemModel*>(sourceModel());
    QString leftPath = sourceModel()->data(source_left, QFileSystemModel::FilePathRole).toString();
    QString rightPath = sourceModel()->data(source_right, QFileSystemModel::FilePathRole).toString();
    bool isLeftDir = fsm->isDir(source_left);
    bool isRightDir = fsm->isDir(source_right);

    if (isLeftDir == isRightDir)
        return (leftPath.compare(rightPath, Qt::CaseInsensitive) < 0);
    if (isLeftDir)
        return true;
    return false;
}
