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

#ifndef PROJECTWIDGET_H
#define PROJECTWIDGET_H

#include <QLineEdit>
#include <QToolButton>

#include "project.h"

#include "projecttreeview.h"

class ProjectWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ProjectWidget(Project *project, QWidget *parent = nullptr);

signals:
    void openedFile(QString fileName);
    void closedFile(QString fileName);

public slots:
    void selectFile(const QString &fileName);

protected slots:
    void openFile(const QString &fileName);
    void closeFile(const QString &fileName);

protected:
    Project *_project;
    ProjectTreeView *_projectView;

    void createMenu();
    QToolButton *_menuBtn;
    QLineEdit *_filterEdit;
};

#endif // PROJECTWIDGET_H
