#include "settingspage.h"

SettingsPage::SettingsPage(const QIcon &icon, const QString &label)
    : _icon(icon), _label(label)
{
}

const QIcon &SettingsPage::icon() const
{
    return _icon;
}

void SettingsPage::setIcon(const QIcon &icon)
{
    _icon = icon;
}

const QString &SettingsPage::label() const
{
    return _label;
}

void SettingsPage::setLabel(const QString &label)
{
    _label = label;
}

void SettingsPage::commit()
{

}
