#ifndef HEXEDITORSETTINGS_H
#define HEXEDITORSETTINGS_H

#include "settingspage.h"

class HexEditorSettings : public SettingsPage
{
    Q_OBJECT
public:
    HexEditorSettings();

    // SettingsPage interface
protected:
    virtual void execCommit();
    virtual void createWidgets();
};

#endif // HEXEDITORSETTINGS_H
