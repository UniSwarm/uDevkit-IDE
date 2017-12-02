#include "settingsmanager.h"

SettingsManager *SettingsManager::settingsManager = Q_NULLPTR;

SettingsManager::SettingsManager(QObject *parent) : QObject(parent)
{
}

SettingsManager *SettingsManager::instance()
{
    if (!settingsManager)
        settingsManager = new SettingsManager();

    return settingsManager;
}

SettingsClass *SettingsManager::registerClass(const QString &className)
{
    SettingsClass *settingClass;

    QMap<QString, SettingsClass*>::const_iterator find = _classesMap.constFind(className);
    if (find != _classesMap.cend())
    {
        settingClass = *find;
    }
    else
    {
        settingClass = new SettingsClass(className);
        _classesMap.insert(className, settingClass);
    }

    return settingClass;
}

Setting *SettingsManager::registerSetting(const QString &className, const QString &name, const QVariant &defaultValue)
{
    SettingsClass *settingClass = registerClass(className);
    return settingClass->registerSetting(name, defaultValue);
}
