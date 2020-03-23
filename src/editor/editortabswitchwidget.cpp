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

#include "editortabswitchwidget.h"

#include <QDebug>
#include <QEvent>
#include <QKeyEvent>

EditorTabSwitchWidget::EditorTabSwitchWidget(QWidget *parent)
    : QListWidget(parent)
{
    setSelectionMode(SingleSelection);
    connect(this, &QListWidget::currentRowChanged, this, &EditorTabSwitchWidget::updateId);
    connect(this, &QListWidget::itemClicked, this, &EditorTabSwitchWidget::clickItem);
}

int EditorTabSwitchWidget::id() const
{
    return _id;
}

void EditorTabSwitchWidget::setId(int id)
{
    _id = id;
    setCurrentRow(_id);
}

void EditorTabSwitchWidget::updateId(int row)
{
    if (_id != -1)
    {
        _id = row;
    }
}

void EditorTabSwitchWidget::clickItem(QListWidgetItem *item)
{
    if (item)
    {
        _id = indexFromItem(item).row();
        hide();
    }
}

bool EditorTabSwitchWidget::event(QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = dynamic_cast<QKeyEvent *>(event);
        if (keyEvent->key() == Qt::Key_Tab || keyEvent->key() == Qt::Key_Backtab)
        {
            if (keyEvent->modifiers() & Qt::ShiftModifier)
            {
                _id--;
                if (_id < 0)
                {
                    _id = count() - 1;
                }
            }
            else
            {
                _id++;
                if (_id >= count())
                {
                    _id = 0;
                }
            }
            setCurrentRow(_id);
            keyEvent->ignore();
            return false;
        }
        else if (keyEvent->key() == Qt::Key_Escape)
        {
            _id = 0;
            setCurrentRow(_id);
            hide();
        }
        return QListWidget::event(event);
    }
    return QListWidget::event(event);
}
