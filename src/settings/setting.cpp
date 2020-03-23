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

#include "setting.h"

Setting::Setting(SettingsClass *parentClass, const QString &name, const QVariant &defaultValue)
    : _parentClass(parentClass)
    , _name(name)
    , _value(defaultValue)
{
    _parentClass = Q_NULLPTR;
}

Setting::Setting(const QString &name, const QVariant &defaultValue)
    : _name(name)
    , _value(defaultValue)
{
}

const QString &Setting::name() const
{
    return _name;
}

const QVariant &Setting::value() const
{
    return _value;
}

void Setting::setValue(const QVariant &value)
{
    if (_value != value)
    {
        _value = value;
        emit valueChanged();
    }
}

SettingsClass *Setting::parentClass() const
{
    return _parentClass;
}

void Setting::setParentClass(SettingsClass *parentClass)
{
    _parentClass = parentClass;
}
