/**
 ** This file is part of the uDevkit-IDE project.
 ** Copyright 2017-2020 UniSwarm
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

#include "projectitemproxymodel.h"

#include "projectitemmodel.h"
#include <QApplication>
#include <QDebug>

#include "project.h"

ProjectItemProxyModel::ProjectItemProxyModel(Project *project)
    : QSortFilterProxyModel()
{
    _project = project;
    setDynamicSortFilter(true);
    setSortRole(ProjectItemModel::FileNameRole);
    sort(1, Qt::AscendingOrder);
    _enabledHiddenFilter = false;
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
    ProjectItemModel *fsm = dynamic_cast<ProjectItemModel *>(sourceModel());
    const QModelIndex index = fsm->index(source_row, 0, source_parent);
    const QString &path = fsm->data(index, ProjectItemModel::FileNameRole).toString();

    // show all dir parent of project path
    /*if (!path.startsWith(_project->rootPath()))
        return true;*/

    // hidden filter: if path match, do not show
    if (_enabledHiddenFilter)
    {
        if (_hiddenFilter.indexIn(path) != -1)
        {
            return false;
        }
    }

    // show filter: show only if match
    if (_showFilter.pattern().size() >= 2)
    {
        if (fsm->isDir(index))
        {
            // QApplication::processEvents();
            for (int i = 0; i < fsm->rowCount(index); i++)
            {
                bool filtered = filterAcceptsRow(i, index);
                if (filtered)
                {
                    return true;
                }
            }
            return false;
        }
        if (_showFilter.indexIn(path) == -1)
        {
            return false;
        }
    }

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
    ProjectItemModel *fsm = dynamic_cast<ProjectItemModel *>(sourceModel());
    QString leftPath = sourceModel()->data(source_left, ProjectItemModel::FileNameRole).toString();
    QString rightPath = sourceModel()->data(source_right, ProjectItemModel::FileNameRole).toString();
    bool isLeftDir = fsm->isDir(source_left);
    bool isRightDir = fsm->isDir(source_right);

    if (isLeftDir == isRightDir)
    {
        return (leftPath.compare(rightPath, Qt::CaseInsensitive) < 0);
    }
    if (isLeftDir)
    {
        return true;
    }
    return false;
}
