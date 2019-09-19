#include "globalsettings.h"

#include <QFormLayout>

GlobalSettings::GlobalSettings()
    : SettingsPage(QIcon(":/icons/img/dark/icons8-settings.png"), tr("Global"))
{
    _settingsClass = rtset()->registerClass("global");
    createWidgets();
}

void GlobalSettings::execCommit()
{
    _settingsClass->setSetting("language", _langComboBox->currentData());
    _settingsClass->commit();
}

void GlobalSettings::createWidgets()
{
    QFormLayout *layout = new QFormLayout();

    _langComboBox = new QComboBox();
    _langComboBox->addItem("English", "en");
    _langComboBox->addItem("Français", "fr");
    int find = _langComboBox->findData(SettingsManager::language());
    if (find != -1)
        _langComboBox->setCurrentIndex(find);
    layout->addRow(tr("Language (restart needed to take effect)"), _langComboBox);

    setLayout(layout);
}
