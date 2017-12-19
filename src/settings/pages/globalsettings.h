#ifndef GLOBALSETTINGS_H
#define GLOBALSETTINGS_H

#include "settingspage.h"

#include <QComboBox>

class GlobalSettings : public SettingsPage
{
    Q_OBJECT
public:
    GlobalSettings();

    // SettingsPage interface
protected:
    virtual void execCommit();
    virtual void createWidgets();

    QComboBox *_langComboBox;
};

#endif // GLOBALSETTINGS_H
