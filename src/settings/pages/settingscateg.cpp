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

#include "settingscateg.h"

SettingsCateg::SettingsCateg(const QIcon &icon, const QString &label)
    : _icon(icon)
    , _label(label)
{
}

SettingsPage *SettingsCateg::page(int i) const
{
    return dynamic_cast<SettingsPage *>(widget(i));
}

void SettingsCateg::addPage(SettingsPage *page)
{
    addTab(page, page->icon(), page->label());
}

const QIcon &SettingsCateg::icon() const
{
    return _icon;
}

void SettingsCateg::setIcon(const QIcon &icon)
{
    _icon = icon;
}

const QString &SettingsCateg::label() const
{
    return _label;
}

void SettingsCateg::setLabel(const QString &label)
{
    _label = label;
}

void SettingsCateg::commitChange()
{
    for (int i = 0; i < count(); i++)
    {
        SettingsPage *page = dynamic_cast<SettingsPage *>(widget(i));
        page->commit();
    }
}
