#ifndef IMAGEEDITORSETTINGS_H
#define IMAGEEDITORSETTINGS_H

#include "settingspage.h"

class ImageEditorSettings : public SettingsPage
{
    Q_OBJECT
public:
    ImageEditorSettings();

    // SettingsPage interface
protected:
    void execCommit() override;
    void createWidgets() override;
};

#endif // IMAGEEDITORSETTINGS_H
