#ifndef HTMLSETTINGS_H
#define HTMLSETTINGS_H

#include "settingspage.h"

class HtmlEditorSettings : public SettingsPage
{
    Q_OBJECT
public:
    HtmlEditorSettings();

    // SettingsPage interface
protected:
    void execCommit() override;
    void createWidgets() override;
};

#endif // HTMLSETTINGS_H
