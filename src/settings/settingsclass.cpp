#include "settingsclass.h"

SettingsClass::SettingsClass(const QString &name)
    : _name(name)
{
}

const QString &SettingsClass::name() const
{
    return _name;
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
