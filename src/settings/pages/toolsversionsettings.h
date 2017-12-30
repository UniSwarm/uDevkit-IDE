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
    virtual void execCommit();
    virtual void createWidgets();

    PathEditWidget *_pathEditWidget;
};

#endif // TOOLSVERSIONSETTINGS_H
