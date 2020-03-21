#ifndef TOOLSVERSIONSETTINGS_H
#define TOOLSVERSIONSETTINGS_H

#include "settingspage.h"

#include "settings/widgets/patheditwidget.h"

class ToolsVersionSettings : public SettingsPage
{
    Q_OBJECT
public:
    ToolsVersionSettings();

    // SettingsPage interface
protected:
    void execCommit() override;
    void createWidgets() override;

    PathEditWidget *_pathEditWidget;
};

#endif // TOOLSVERSIONSETTINGS_H
