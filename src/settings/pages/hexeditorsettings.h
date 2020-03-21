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
    void execCommit() override;
    void createWidgets() override;
};

#endif // HEXEDITORSETTINGS_H
