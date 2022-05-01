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

#ifndef PROJECTTREEVIEW_H
#define PROJECTTREEVIEW_H

#include <QAction>
#include <QTreeView>

#include "project.h"
#include "projectitemproxymodel.h"

class ProjectTreeView : public QTreeView
{
    Q_OBJECT
public:
    ProjectTreeView(Project *project, QWidget *parent = nullptr);

    ProjectItemProxyModel *proxy() const;

public slots:
    void selectFile(const QString &fileName);

    void setHiddenFilter(const QRegularExpression &regExp);
    void setHiddenFilter(const QString &pattern);
    void enableHiddenFilter(bool enable = true);

    void setShowFilter(const QRegularExpression &regExp);
    void setShowFilter(const QString &pattern);

    void remove();
    void rename();

signals:
    void openedFile(QString fileName);
    void closedFile(QString fileName);

protected:
    Project *_project;
    ProjectItemProxyModel *_proxy;

    QAction *_removeAction;
    QAction *_fileRenameAction;

    // QWidget interface
protected:
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
};

#endif  // PROJECTTREEVIEW_H
