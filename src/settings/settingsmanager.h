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
