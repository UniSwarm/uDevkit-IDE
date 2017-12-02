#ifndef SETTINGSCLASS_H
#define SETTINGSCLASS_H

#include <QObject>
#include <QString>
#include <QMap>

#include "setting.h"

class SettingsClass : public QObject
{
    Q_OBJECT
public:
    SettingsClass(const QString &name);

    const QString &name() const;

    Setting *registerSetting(const QString &name, const QVariant &defaultValue=QVariant());

    bool isModified() const;

signals:
    void classModified();

public slots:
    void commit();

protected:
    QString _name;
    QMap<QString, Setting*> _settingsMap;
    bool _modified;
};

#endif // SETTINGSCLASS_H
