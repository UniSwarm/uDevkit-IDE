/**
 ** This file is part of the uDevkit-IDE project.
 ** Copyright 2017-2020 UniSwarm
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

#include "toolsversionsettings.h"

#include <QFormLayout>
#include <QGroupBox>

ToolsVersionSettings::ToolsVersionSettings()
    : SettingsPage(QIcon(":/icons/img/dark/icons8-toolbox.png"), tr("Tools version"))
{
    _settingsClass = rtset()->registerClass("tools");
    createWidgets();
}

void ToolsVersionSettings::execCommit()
{
    _settingsClass->setSetting("make/path", _pathEditWidget->path());
    _settingsClass->commit();
}

void ToolsVersionSettings::createWidgets()
{
    QVBoxLayout *layout = new QVBoxLayout();
    layout->setContentsMargins(5, 0, 5, 0);

    QGroupBox *makeBox = new QGroupBox(tr("GNU Make"));
    QFormLayout *makeLayout = new QFormLayout();
    makeLayout->setLabelAlignment(Qt::AlignVCenter | Qt::AlignLeft);

    _pathEditWidget = new PathEditWidget();
    _pathEditWidget->setProgramm("make");
    _pathEditWidget->setPath(_settingsClass->setting("make/path").toString());
    _pathEditWidget->setVersionArg("--version");
    _pathEditWidget->checkProgramm();
    makeLayout->addRow(tr("Make path"), _pathEditWidget);
    makeLayout->labelForField(_pathEditWidget)->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    makeBox->setLayout(makeLayout);
    layout->addWidget(makeBox);

    layout->addSpacerItem(new QSpacerItem(10, 20, QSizePolicy::Minimum, QSizePolicy::Expanding));
    setLayout(layout);
}
