#include "settingsmanager.h"

#include <QApplication>
#include <QDebug>

SettingsManager *SettingsManager::settingsManager = Q_NULLPTR;

SettingsManager::SettingsManager(QObject *parent)
    : SettingsClass("", parent)
{
}

SettingsManager *SettingsManager::instance()
{
    if (!settingsManager)
        settingsManager = new SettingsManager();

    return settingsManager;
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
    return instance()->registerSetting("global/language", QLocale::system().name().mid(0,2))->value().toString();
}

void SettingsManager::load(SettingsClass *settingsClass)
{
    QSettings settings(QApplication::organizationName(), QApplication::applicationName());
    settings.beginGroup(settingsClass->name());
    foreach (QString key, settings.allKeys())
    {
        settingsClass->registerSetting(key, settings.value(key));
    }
    settings.endGroup();
}
