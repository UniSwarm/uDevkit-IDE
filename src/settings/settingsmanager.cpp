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

#include "settingsmanager.h"

#include <QApplication>
#include <QDebug>

SettingsManager *SettingsManager::settingsManager = nullptr;

SettingsManager::SettingsManager(QObject *parent)
    : SettingsClass("", parent)
{
}

SettingsManager *SettingsManager::instance()
{
    if (settingsManager == nullptr)
    {
        settingsManager = new SettingsManager();
    }

    return settingsManager;
}

void SettingsManager::save()
{
    QSettings settings(QApplication::organizationName(), QApplication::applicationName());
    for (SettingsClass *settingClass : instance()->_classesMap)
    {
        settingClass->save(&settings);
    }
}

QString SettingsManager::language()
{
    return instance()->registerSetting("global/language", QLocale::system().name().mid(0, 2))->value().toString();
}

QString SettingsManager::userName()
{
    return instance()->registerSetting("global/username", "username")->value().toString();
}

QString SettingsManager::userPseudo()
{
    return instance()->registerSetting("global/userpseudo", "userpseudo")->value().toString();
}

void SettingsManager::load(SettingsClass *settingsClass)
{
    QSettings settings(QApplication::organizationName(), QApplication::applicationName());
    settings.beginGroup(settingsClass->name());
    for (const QString &key : settings.allKeys())
    {
        settingsClass->registerSetting(key, settings.value(key));
    }
    settings.endGroup();
}
