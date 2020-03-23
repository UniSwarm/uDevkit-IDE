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

#ifndef EDITORTABSWITCHWIDGET_H
#define EDITORTABSWITCHWIDGET_H

#include <QListWidget>

class EditorTabSwitchWidget : public QListWidget
{
    Q_OBJECT
public:
    EditorTabSwitchWidget(QWidget *parent);

    int id() const;
    void setId(int id);

protected slots:
    void updateId(int row);
    void clickItem(QListWidgetItem *item);

    // QObject interface
public:
    bool event(QEvent *event) override;

protected:
    int _id;
};

#endif // EDITORTABSWITCHWIDGET_H
