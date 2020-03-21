#include "codeeditorsettings.h"

#include <QDebug>
#include <QFormLayout>

CodeEditorSettings::CodeEditorSettings()
    : SettingsPage(QIcon(":/icons/img/dark/icons8-code.png"), tr("Code editor"))
{
    _settingsClass = rtset()->registerClass("codeEditor");
    createWidgets();
}

void CodeEditorSettings::updateTest()
{
    SettingsClass sclass(*_settingsClass);
    sclass.setSetting("fontFamily", _fontComboBox->currentFont().family());
    sclass.setSetting("fontSize", _fontSizeSpinBox->value());
    _editorTest->setSettingsClass(&sclass);
}

void CodeEditorSettings::execCommit()
{
    _settingsClass->setSetting("fontFamily", _fontComboBox->currentFont().family());
    _settingsClass->setSetting("fontSize", _fontSizeSpinBox->value());
    _settingsClass->commit();
}

void CodeEditorSettings::createWidgets()
{
    QFormLayout *layout = new QFormLayout();

    _fontComboBox = new QFontComboBox();
    QFont font;
    font.setFamily(_settingsClass->setting("fontFamily", "monospace").toString());
    _fontComboBox->setFontFilters(QFontComboBox::MonospacedFonts);
    _fontComboBox->setCurrentFont(font);
    connect(_fontComboBox, &QFontComboBox::currentFontChanged, this, &CodeEditorSettings::updateTest);
    layout->addRow(tr("Font family"), _fontComboBox);

    _fontSizeSpinBox = new QSpinBox();
    _fontSizeSpinBox->setRange(4, 50);
    _fontSizeSpinBox->setValue(_settingsClass->setting("fontSize", 10).toInt());
    connect(_fontSizeSpinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=]() { updateTest(); });
    layout->addRow(tr("Font size"), _fontSizeSpinBox);

    _editorTest = new CodeEditor(Q_NULLPTR);
    layout->addWidget(_editorTest);
    _editorTest->active();
    _editorTest->setText("int main()\n\
{\n\
    return 0;\n\
}");
    _editorTest->setGrammar("c");
    updateTest();

    setLayout(layout);
}
