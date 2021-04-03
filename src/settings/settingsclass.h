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

#ifndef SETTINGSCLASS_H
#define SETTINGSCLASS_H

#include <QMap>
#include <QObject>
#include <QSettings>
#include <QString>

#include "setting.h"

class SettingsClass : public QObject
{
    Q_OBJECT
public:
    SettingsClass(const QString &name, QObject *parent = Q_NULLPTR);
    SettingsClass(const SettingsClass &other);
    ~SettingsClass() override;

    const QString &name() const;
    QVariant setting(const QString &name, const QVariant &defaultValue = QVariant());
    void setSetting(const QString &name, const QVariant &value);
    Setting *registerSetting(const QString &name, const QVariant &defaultValue = QVariant());
    Setting *getSetting(const QString &path);

    SettingsClass *registerClass(const QString &className);
    SettingsClass *getClass(const QString &className);

    bool isModified() const;

    // protected:
    void save(QSettings *settings);

signals:
    void classModified();

public slots:
    void commit();

protected:
    QString _name;
    QMap<QString, Setting *> _settingsMap;
    QMap<QString, SettingsClass *> _classesMap;
    bool _modified;
};

#endif // SETTINGSCLASS_H
