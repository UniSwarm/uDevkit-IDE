/**
 ** This file is part of the uDevkit-IDE project.
 ** Copyright 2017-2021 UniSwarm
 **
 ** This program is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU General Public License as published by
 ** the Free Software Foundation, either version 3 of the License, or
 ** (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 ** GNU General Public License for more details.
 **
 ** You should have received a copy of the GNU General Public License
 ** along with this program. If not, see <http://www.gnu.org/licenses/>.
 **/

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
    _settingsClass->setSetting("username", _usernameEdit->text());
    _settingsClass->setSetting("userpseudo", _userpseudoEdit->text());
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
    {
        _langComboBox->setCurrentIndex(find);
    }
    layout->addRow(tr("Language (restart needed to take effect)"), _langComboBox);

    _usernameEdit = new QLineEdit();
    _usernameEdit->setText(SettingsManager::userName());
    layout->addRow(tr("Username (First name, Last name)"), _usernameEdit);

    _userpseudoEdit = new QLineEdit();
    _userpseudoEdit->setText(SettingsManager::userPseudo());
    layout->addRow(tr("Github pseudo (pseudo)"), _userpseudoEdit);

    setLayout(layout);
}
