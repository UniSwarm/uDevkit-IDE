#ifndef SETTINGSCLASS_H
#define SETTINGSCLASS_H

#include <QMap>
#include <QObject>
#include <QSettings>
#include <QString>

#include "setting.h"

class SettingsClass : public QObject
{
    Q_OBJECT
public:
    SettingsClass(const QString &name, QObject *parent = Q_NULLPTR);
    SettingsClass(const SettingsClass &other);
    ~SettingsClass() override;

    const QString &name() const;
    QVariant setting(const QString &name, const QVariant &defaultValue = QVariant());
    void setSetting(const QString &name, const QVariant &value);
    Setting *registerSetting(const QString &name, const QVariant &defaultValue = QVariant());
    Setting *getSetting(const QString &path);

    SettingsClass *registerClass(const QString &className);
    SettingsClass *getClass(const QString &className);

    bool isModified() const;

    // protected:
    void save(QSettings *settings);

signals:
    void classModified();

public slots:
    void commit();

protected:
    QString _name;
    QMap<QString, Setting *> _settingsMap;
    QMap<QString, SettingsClass *> _classesMap;
    bool _modified;
};

#endif // SETTINGSCLASS_H
