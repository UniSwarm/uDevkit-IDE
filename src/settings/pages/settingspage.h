#ifndef SETTINGSPAGE_H
#define SETTINGSPAGE_H

#include <QWidget>
#include <QIcon>

#include "settings/settingsmanager.h"

class SettingsPage : public QWidget
{
    Q_OBJECT
public:
    explicit SettingsPage(const QIcon &icon=QIcon(), const QString &label=QString());

    const QIcon &icon() const;
    void setIcon(const QIcon &icon);

    const QString &label() const;
    void setLabel(const QString &label);

public slots:
    void commit();

protected:
    QIcon _icon;
    QString _label;

    virtual void execCommit() =0;
    virtual void createWidgets() =0;

    SettingsClass *_settingsClass;
};

#endif // SETTINGSPAGE_H
