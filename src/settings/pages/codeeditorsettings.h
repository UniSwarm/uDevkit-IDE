#ifndef CODEEDITORSETTINGS_H
#define CODEEDITORSETTINGS_H

#include "settingspage.h"

#include <QFontComboBox>
#include <QSpinBox>

class CodeEditorSettings : public SettingsPage
{
    Q_OBJECT
public:
    CodeEditorSettings();

    // SettingsPage interface
protected:
    virtual void execCommit();
    virtual void createWidgets();

    QFontComboBox *_fontComboBox;
    QSpinBox *_fontSizeSpinBox;

    SettingsClass *_settingsClass;
};

#endif // CODEEDITORSETTINGS_H
