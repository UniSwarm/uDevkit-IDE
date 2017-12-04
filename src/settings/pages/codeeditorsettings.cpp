#include "codeeditorsettings.h"

#include <QFormLayout>

CodeEditorSettings::CodeEditorSettings()
    : SettingsPage(QIcon(), "Code editor")
{
    _settingsClass = SettingsManager::registerClass("codeEditor");
    createWidgets();
}


void CodeEditorSettings::execCommit()
{
    _settingsClass->setSetting("fontFamily", _fontComboBox->font().family());
    _settingsClass->setSetting("fontSize", _fontSizeSpinBox->value());
    _settingsClass->commit();
}

void CodeEditorSettings::createWidgets()
{
    QFormLayout *layout = new QFormLayout();

    _fontComboBox = new QFontComboBox();
    QFont font;
    font.setFamily(_settingsClass->setting("fontFamily", "monospace").toString());
    _fontComboBox->setFont(font);
    layout->addRow(tr("Font family"), _fontComboBox);

    _fontSizeSpinBox = new QSpinBox();
    _fontSizeSpinBox->setRange(4, 50);
    _fontSizeSpinBox->setValue(_settingsClass->setting("fontSize", 10).toInt());
    layout->addRow(tr("Font size"), _fontSizeSpinBox);

    setLayout(layout);
}
