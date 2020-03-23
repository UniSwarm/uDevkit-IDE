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

#include "settingspage.h"

SettingsPage::SettingsPage(const QIcon &icon, const QString &label)
    : _icon(icon)
    , _label(label)
{
}

const QIcon &SettingsPage::icon() const
{
    return _icon;
}

void SettingsPage::setIcon(const QIcon &icon)
{
    _icon = icon;
}

const QString &SettingsPage::label() const
{
    return _label;
}

void SettingsPage::setLabel(const QString &label)
{
    _label = label;
}

void SettingsPage::commit()
{
    execCommit();
}
