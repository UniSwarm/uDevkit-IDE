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

#include "settingsclass.h"

#include "settingsmanager.h"

#include <QDebug>
#include <utility>

SettingsClass::SettingsClass(QString name, QObject *parent)
    : QObject(parent)
    , _name(std::move(name))
{
}

SettingsClass::SettingsClass(const SettingsClass &other)
{
    _name = other.name();
    _modified = false;
    for (Setting *setting : other._settingsMap)
    {
        registerSetting(setting->name(), setting->value());
    }
    for (SettingsClass *settingClass : other._classesMap)
    {
        SettingsClass *newSettingClass = new SettingsClass(settingClass->name());
        _classesMap.insert(newSettingClass->name(), newSettingClass);
    }
}

SettingsClass::~SettingsClass()
{
    qDeleteAll(_settingsMap);
    qDeleteAll(_classesMap);
}

QVariant SettingsClass::setting(const QString &name, const QVariant &defaultValue)
{
    Setting *setting = registerSetting(name, defaultValue);
    return setting->value();
}

const QString &SettingsClass::name() const
{
    return _name;
}

SettingsClass *SettingsClass::registerClass(const QString &className)
{
    SettingsClass *settingClass;

    QMap<QString, SettingsClass *>::const_iterator find = _classesMap.constFind(className);
    if (find != _classesMap.cend())
    {
        settingClass = *find;
    }
    else
    {
        settingClass = new SettingsClass(className);
        _classesMap.insert(className, settingClass);

        // load
        SettingsManager::instance()->load(settingClass);
    }

    return settingClass;
}

void SettingsClass::setSetting(const QString &name, const QVariant &value)
{
    Setting *setting = registerSetting(name, value);
    setting->setValue(value);
}

Setting *SettingsClass::registerSetting(const QString &path, const QVariant &defaultValue)
{
    int pos = path.indexOf('/');
    if (pos == -1)
    {
        Setting *setting = nullptr;
        QMap<QString, Setting *>::const_iterator find = _settingsMap.constFind(path);
        if (find != _settingsMap.cend())
        {
            setting = *find;
        }
        else
        {
            setting = new Setting(this, path, defaultValue);
            _settingsMap.insert(path, setting);
        }
        return setting;
    }

    QMap<QString, SettingsClass *>::const_iterator find = _classesMap.constFind(path.mid(0, pos));
    if (find != _classesMap.cend())
    {
        return (*find)->registerSetting(path.mid(pos + 1), defaultValue);
    }
    else
    {
        QString className = path.mid(0, pos);
        SettingsClass *settingClass;
        QMap<QString, SettingsClass *>::const_iterator find = _classesMap.constFind(className);
        if (find != _classesMap.cend())
        {
            settingClass = *find;
        }
        else
        {
            settingClass = new SettingsClass(className);
            _classesMap.insert(className, settingClass);

            // load
            SettingsManager::instance()->load(settingClass);
        }
        return settingClass->registerSetting(path.mid(pos + 1), defaultValue);
    }
}

Setting *SettingsClass::getSetting(const QString &path)
{
    int pos = path.indexOf('/');
    if (pos == -1)
    {
        QMap<QString, Setting *>::const_iterator find = _settingsMap.constFind(path);
        if (find != _settingsMap.cend())
        {
            return *find;
        }
    }
    else
    {
        QMap<QString, SettingsClass *>::const_iterator find = _classesMap.constFind(path.mid(0, pos));
        if (find != _classesMap.cend())
        {
            return (*find)->getSetting(path.mid(pos + 1));
        }
    }

    return nullptr;
}

void SettingsClass::commit()
{
    if (_modified)
    {
        emit classModified();
        _modified = false;
    }
}

bool SettingsClass::isModified() const
{
    return _modified;
}

void SettingsClass::save(QSettings *settings)
{
    settings->beginGroup(_name);
    for (Setting *setting : _settingsMap)
    {
        settings->setValue(setting->name(), setting->value());
    }
    for (SettingsClass *settingClass : _classesMap)
    {
        settingClass->save(settings);
    }
    settings->endGroup();
}
