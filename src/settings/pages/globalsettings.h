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
    void execCommit() override;
    void createWidgets() override;

    QComboBox *_langComboBox;
};

#endif // GLOBALSETTINGS_H
