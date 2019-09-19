#include "gitversionsettings.h"

#include <QFormLayout>

GitVersionSettings::GitVersionSettings()
    : SettingsPage(QIcon(":/icons/img/dark/icons8-git.png"), tr("Git version"))
{
    _settingsClass = rtset()->registerClass("gitversion");
    createWidgets();
}

void GitVersionSettings::execCommit()
{
    _settingsClass->setSetting("path", _pathEditWidget->path());
    _settingsClass->commit();
}

void GitVersionSettings::createWidgets()
{
    QFormLayout *layout = new QFormLayout();
    layout->setLabelAlignment(Qt::AlignVCenter | Qt::AlignLeft);

    _pathEditWidget = new PathEditWidget();
    _pathEditWidget->setProgramm("git");
    _pathEditWidget->setPath(_settingsClass->setting("path").toString());
    _pathEditWidget->setVersionArg("--version");
    _pathEditWidget->checkProgramm();
    layout->addRow(tr("Git path"), _pathEditWidget);
    layout->labelForField(_pathEditWidget)->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    setLayout(layout);
}
