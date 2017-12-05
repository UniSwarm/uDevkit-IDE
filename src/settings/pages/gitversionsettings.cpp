#include "gitversionsettings.h"

#include <QFormLayout>

GitVersionSettings::GitVersionSettings()
    : SettingsPage(QIcon(":/icons/img/dark/icons8-git.png"), "Git version")
{
    createWidgets();
}

void GitVersionSettings::execCommit()
{

}

void GitVersionSettings::createWidgets()
{
    QFormLayout *layout = new QFormLayout();
    layout->setLabelAlignment(Qt::AlignVCenter | Qt::AlignLeft);

    _pathEditWidget = new PathEditWidget();
    _pathEditWidget->setProgramm("git");
    _pathEditWidget->setPath("");
    _pathEditWidget->setVersionArg("--version");
    _pathEditWidget->checkProgramm();
    layout->addRow(tr("Git path"), _pathEditWidget);
    layout->labelForField(_pathEditWidget)->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    setLayout(layout);
}
