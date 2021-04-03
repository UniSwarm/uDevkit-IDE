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

#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QObject>
#include <QSettings>

#include "settingsclass.h"

class SettingsManager : public SettingsClass
{
    Q_OBJECT
protected:
    SettingsManager(QObject *parent = Q_NULLPTR);

public:
    static SettingsManager *instance();

    static void save();
    void load(SettingsClass *settingsClass);

    // helpers
    static QString language();

protected:
    static SettingsManager *settingsManager;
};

#define rtset() SettingsManager::instance()

#endif // SETTINGSMANAGER_H
