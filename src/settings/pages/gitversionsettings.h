#ifndef GITVERSIONSETTINGS_H
#define GITVERSIONSETTINGS_H

#include "settingspage.h"

#include "settings/widgets/patheditwidget.h"

class GitVersionSettings : public SettingsPage
{
    Q_OBJECT
public:
    GitVersionSettings();

    // SettingsPage interface
protected:
    void execCommit() override;
    void createWidgets() override;

    PathEditWidget *_pathEditWidget;
};

#endif // GITVERSIONSETTINGS_H
