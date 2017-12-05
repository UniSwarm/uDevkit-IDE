#include "settingsclass.h"

SettingsClass::SettingsClass(const QString &name)
    : _name(name)
{
}

const QString &SettingsClass::name() const
{
    return _name;
}

QVariant SettingsClass::setting(const QString &name, const QVariant &defaultValue)
{
    QMap<QString, Setting*>::const_iterator find = _settingsMap.constFind(name);
    if (find != _settingsMap.cend())
    {
        return (*find)->value();
    }
    registerSetting(name, defaultValue);
    return defaultValue;
}

void SettingsClass::setSetting(const QString &name, const QVariant &value)
{
    QMap<QString, Setting*>::const_iterator find = _settingsMap.constFind(name);
    _modified = true;
    if (find != _settingsMap.cend())
    {
        (*find)->setValue(value);
        return;
    }
    registerSetting(name, value);
}

Setting *SettingsClass::registerSetting(const QString &name, const QVariant &defaultValue)
{
    Setting *setting;

    QMap<QString, Setting*>::const_iterator find = _settingsMap.constFind(name);
    if (find != _settingsMap.cend())
    {
        setting = *find;
    }
    else
    {
        setting = new Setting(this, name, defaultValue);
        _settingsMap.insert(name, setting);
    }

    return setting;
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
        settings->setValue(setting->name() ,setting->value());
    }
    settings->endGroup();
}
