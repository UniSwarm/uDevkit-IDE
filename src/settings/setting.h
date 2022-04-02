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

#ifndef SETTING_H
#define SETTING_H

#include <QObject>
#include <QVariant>

class SettingsClass;

class Setting : public QObject
{
    Q_OBJECT
protected:
    Setting(SettingsClass *_parentClass, QString name, QVariant defaultValue = QVariant());
    Setting(QString name, QVariant defaultValue = QVariant());

public:
    const QString &name() const;

    const QVariant &value() const;
    void setValue(const QVariant &value);

    SettingsClass *parentClass() const;
    void setParentClass(SettingsClass *parentClass);

signals:
    void valueChanged();

protected:
    SettingsClass *_parentClass;
    QString _name;
    QVariant _value;

    friend class SettingsClass;
};

#endif  // SETTING_H
