#include "settingscateg.h"

SettingsCateg::SettingsCateg(const QIcon &icon, const QString &label)
    : _icon(icon), _label(label)
{
}

SettingsPage *SettingsCateg::page(int i) const
{
    return static_cast<SettingsPage *>(widget(i));
}

void SettingsCateg::addPage(SettingsPage *page)
{
    addTab(page, page->icon(), page->label());
}

const QIcon &SettingsCateg::icon() const
{
    return _icon;
}

void SettingsCateg::setIcon(const QIcon &icon)
{
    _icon = icon;
}

const QString &SettingsCateg::label() const
{
    return _label;
}

void SettingsCateg::setLabel(const QString &label)
{
    _label = label;
}

void SettingsCateg::commitChange()
{
    for (int i=0; i<count(); i++)
    {
        SettingsPage *page = static_cast<SettingsPage *>(widget(i));
        page->commit();
    }
}
