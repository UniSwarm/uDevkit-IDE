#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QObject>
#include <QSettings>

#include "settingsclass.h"

class SettingsManager : public QObject
{
    Q_OBJECT
protected:
    SettingsManager(QObject *parent = nullptr);

public:
    static SettingsManager *instance();

    static SettingsClass *registerClass(const QString &className);
    static Setting *registerSetting(const QString &className, const QString &name, const QVariant &defaultValue);

    static void save();

protected:
    static SettingsManager *settingsManager;

    QMap<QString, SettingsClass*> _classesMap;
    QSettings _settings;
};

#endif // SETTINGSMANAGER_H
