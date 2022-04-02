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

#ifndef SETTINGSCATEG_H
#define SETTINGSCATEG_H

#include <QTabWidget>

#include "settingspage.h"

class SettingsCateg : public QTabWidget
{
    Q_OBJECT
public:
    SettingsCateg(QIcon icon = QIcon(), QString label = QString());

    SettingsPage *page(int i) const;
    void addPage(SettingsPage *page);

    const QIcon &icon() const;
    void setIcon(const QIcon &icon);

    const QString &label() const;
    void setLabel(const QString &label);

public slots:
    void commitChange();

protected:
    QIcon _icon;
    QString _label;
};

#endif  // SETTINGSCATEG_H
