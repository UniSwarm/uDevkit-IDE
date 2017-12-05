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
    virtual void execCommit();
    virtual void createWidgets();
};

#endif // IMAGEEDITORSETTINGS_H
