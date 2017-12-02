#ifndef SETTING_H
#define SETTING_H

#include <QObject>
#include <QVariant>

class SettingsClass;

class Setting : public QObject
{
    Q_OBJECT
protected:
    Setting(SettingsClass *_parentClass, const QString &name, const QVariant &defaultValue=QVariant());
    Setting(const QString &name, const QVariant &defaultValue=QVariant());

public:
    const QString &name() const;

    const QVariant &value() const;
    void setValue(const QVariant &value);

    SettingsClass *parentClass() const;
    void setParentClass(SettingsClass *parentClass);

signals:
    void valueChanged();

protected:
    SettingsClass *_parentClass;
    QString _name;
    QVariant _value;

    friend class SettingsClass;
};

#endif // SETTING_H
