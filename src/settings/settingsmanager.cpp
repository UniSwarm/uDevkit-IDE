#include "settingsmanager.h"

#include <QApplication>

SettingsManager *SettingsManager::settingsManager = Q_NULLPTR;

SettingsManager::SettingsManager(QObject *parent)
    : QObject(parent)
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

    QMap<QString, SettingsClass*>::const_iterator find = instance()->_classesMap.constFind(className);
    if (find != instance()->_classesMap.cend())
    {
        settingClass = *find;
    }
    else
    {
        settingClass = new SettingsClass(className);
        instance()->_classesMap.insert(className, settingClass);

        // load
        QSettings settings(QApplication::organizationName(), QApplication::applicationName());
        settings.beginGroup(className);
        foreach (QString key, settings.allKeys())
        {
            settingClass->registerSetting(key, settings.value(key));
        }
        settings.endGroup();
    }

    return settingClass;
}

Setting *SettingsManager::registerSetting(const QString &className, const QString &name, const QVariant &defaultValue)
{
    SettingsClass *settingClass = registerClass(className);
    return settingClass->registerSetting(name, defaultValue);
}

void SettingsManager::save()
{
    QSettings settings(QApplication::organizationName(), QApplication::applicationName());
    foreach (SettingsClass *settingClass, instance()->_classesMap)
    {
        settingClass->save(&settings);
    }
}

QString SettingsManager::language()
{
    return registerSetting("global", "language", QLocale::system().name().mid(0,2))->value().toString();
}
