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
    virtual void execCommit();
    virtual void createWidgets();
};

#endif // HTMLSETTINGS_H
