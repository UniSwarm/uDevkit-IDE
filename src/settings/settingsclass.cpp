#include "settingsclass.h"

#include "settingsmanager.h"

#include <QDebug>

SettingsClass::SettingsClass(const QString &name, QObject *parent)
    : QObject(parent)
    , _name(name)
{
}

SettingsClass::SettingsClass(const SettingsClass &other)
{
    _name = other.name();
    _modified = false;
    foreach (Setting *setting, other._settingsMap)
        registerSetting(setting->name(), setting->value());
    foreach (SettingsClass *settingClass, other._classesMap)
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
        Setting *setting = Q_NULLPTR;
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
    else
    {
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

    return Q_NULLPTR;
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
    foreach (Setting *setting, _settingsMap)
    {
        settings->setValue(setting->name(), setting->value());
    }
    foreach (SettingsClass *settingClass, _classesMap)
    {
        settingClass->save(settings);
    }
    settings->endGroup();
}
