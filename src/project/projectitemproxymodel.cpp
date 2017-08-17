#include "projectitemproxymodel.h"

#include <QDebug>
#include "projectitemmodel.h"

#include "project.h"

ProjectItemProxyModel::ProjectItemProxyModel(Project *project)
    : QSortFilterProxyModel()
{
    _project = project;
    setDynamicSortFilter(true);
    setSortRole(ProjectItemModel::FileNameRole);
    sort(1, Qt::AscendingOrder);
}

void ProjectItemProxyModel::setHiddenFilter(const QRegExp &regExp)
{
    _hiddenFilter = regExp;
    invalidateFilter();
}

void ProjectItemProxyModel::setHiddenFilter(const QString &pattern)
{
    _hiddenFilter = QRegExp(pattern, Qt::CaseInsensitive, QRegExp::RegExp);
    invalidateFilter();
}

void ProjectItemProxyModel::enableHiddenFilter(bool enable)
{
    _enabledHiddenFilter = enable;
    invalidateFilter();
}

void ProjectItemProxyModel::setShowFilter(const QRegExp &regExp)
{
    _showFilter = regExp;
    invalidateFilter();
}

void ProjectItemProxyModel::setShowFilter(const QString &pattern)
{
    _showFilter = QRegExp(pattern, Qt::CaseInsensitive, QRegExp::RegExp);
    invalidateFilter();
}

bool ProjectItemProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    ProjectItemModel *fsm = static_cast<ProjectItemModel*>(sourceModel());
    const QModelIndex index = fsm->index(source_row, 0, source_parent);
    const QString &path = fsm->data(index, ProjectItemModel::FilePathRole).toString();

    // show all dir parent of project path
    if (!path.startsWith(_project->rootPath()))
        return true;

    // hidden filter: if path match, do not show
    if (_enabledHiddenFilter)
    {
        if (_hiddenFilter.indexIn(path) != -1)
            return false;
    }

    // show filter: show only if match
    if (fsm->isDir(index))
        return true; // TODO add recursive search
    if (_showFilter.indexIn(path) == -1)
        return false;

    return true;
}

bool ProjectItemProxyModel::filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const
{
    Q_UNUSED(source_column)
    Q_UNUSED(source_parent)
    return true;
}

bool ProjectItemProxyModel::lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const
{
    ProjectItemModel *fsm = static_cast<ProjectItemModel*>(sourceModel());
    QString leftPath = sourceModel()->data(source_left, ProjectItemModel::FileNameRole).toString();
    QString rightPath = sourceModel()->data(source_right, ProjectItemModel::FileNameRole).toString();
    bool isLeftDir = fsm->isDir(source_left);
    bool isRightDir = fsm->isDir(source_right);

    if (isLeftDir == isRightDir)
        return (leftPath.compare(rightPath, Qt::CaseInsensitive) < 0);
    if (isLeftDir)
        return true;
    return false;
}
