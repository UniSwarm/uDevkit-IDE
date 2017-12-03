#ifndef SETTINGSCATEG_H
#define SETTINGSCATEG_H

#include <QTabWidget>

#include "settingspage.h"

class SettingsCateg : public QTabWidget
{
    Q_OBJECT
public:
    SettingsCateg(const QIcon &icon=QIcon(), const QString &label=QString());

    SettingsPage *page(int i) const;
    void addPage(SettingsPage *page);

    const QIcon &icon() const;
    void setIcon(const QIcon &icon);

    const QString &label() const;
    void setLabel(const QString &label);

public slots:
    void commitChange();

protected:
    QIcon _icon;
    QString _label;
};

#endif // SETTINGSCATEG_H
