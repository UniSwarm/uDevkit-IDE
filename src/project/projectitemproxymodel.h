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

#ifndef PROJECTITEMPROXYMODEL_H
#define PROJECTITEMPROXYMODEL_H

#include <QSortFilterProxyModel>

class Project;

class ProjectItemProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit ProjectItemProxyModel(Project *project);

public slots:
    void setHiddenFilter(const QRegExp &regExp);
    void setHiddenFilter(const QString &pattern);
    void enableHiddenFilter(bool enable = true);

    void setShowFilter(const QRegExp &regExp);
    void setShowFilter(const QString &pattern);

    // QSortFilterProxyModel interface
protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
    bool filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const override;

protected:
    Project *_project;
    QRegExp _hiddenFilter;
    QRegExp _showFilter;
    bool _enabledHiddenFilter;

    // QSortFilterProxyModel interface
protected:
    bool lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const override;
};

#endif  // PROJECTITEMPROXYMODEL_H
