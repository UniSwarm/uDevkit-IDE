#include "toolsversionsettings.h"

#include <QFormLayout>
#include <QLayout>
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
    makeLayout->addRow(tr("path"), _pathEditWidget);
    makeLayout->labelForField(_pathEditWidget)->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    makeBox->setLayout(makeLayout);
    layout->addWidget(makeBox);

    layout->addSpacerItem(new QSpacerItem(10, 20, QSizePolicy::Minimum, QSizePolicy::Expanding));
    setLayout(layout);
}
