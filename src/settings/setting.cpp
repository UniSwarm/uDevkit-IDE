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
