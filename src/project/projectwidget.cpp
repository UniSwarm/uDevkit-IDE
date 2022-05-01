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

#include "projectwidget.h"

#include <QAction>
#include <QApplication>
#include <QBoxLayout>
#include <QDebug>
#include <QMenu>

ProjectWidget::ProjectWidget(Project *project, QWidget *parent)
    : QWidget(parent)
{
    _project = project;

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);

    QHBoxLayout *filterLayout = new QHBoxLayout();
    filterLayout->setContentsMargins(0, 0, 0, 0);

    _filterEdit = new QLineEdit();
    _filterEdit->setClearButtonEnabled(true);
    _filterEdit->setPlaceholderText(tr("Search files..."));
    filterLayout->addWidget(_filterEdit);
    _menuBtn = new QToolButton();
    _menuBtn->setText("...");
    _menuBtn->setPopupMode(QToolButton::InstantPopup);
    filterLayout->addWidget(_menuBtn);

    layout->addItem(filterLayout);

    _projectView = new ProjectTreeView(_project, this);
    _projectView->setHiddenFilter(QRegularExpression("(nbproject|bin|.*build.*|.*\\.git$|rtsim)", QRegularExpression::CaseInsensitiveOption));
    _projectView->proxy()->enableHiddenFilter(false);

    layout->addWidget(_projectView);
    setLayout(layout);

    connect(_projectView, &ProjectTreeView::openedFile, this, &ProjectWidget::openFile);
    connect(_projectView, &ProjectTreeView::closedFile, this, &ProjectWidget::closeFile);

    connect(_filterEdit, &QLineEdit::textChanged, [=](const QString &text)
    {
        _projectView->proxy()->setShowFilter(text);
    });

    createMenu();
}

void ProjectWidget::selectFile(const QString &fileName)
{
    _projectView->selectFile(fileName);
}

void ProjectWidget::openFile(const QString &fileName)
{
    emit openedFile(fileName);
}

void ProjectWidget::closeFile(const QString &fileName)
{
    emit closedFile(fileName);
}

void ProjectWidget::createMenu()
{
    QMenu *menu = new QMenu();

    QAction *enFilter = menu->addAction(tr("enable filter"));
    enFilter->setCheckable(true);
    enFilter->setChecked(false);
    connect(enFilter, &QAction::toggled, _projectView->proxy(), &ProjectItemProxyModel::enableHiddenFilter);

    _menuBtn->setMenu(menu);
}
