#include "hexeditor.h"

#include <QApplication>
#include <QBoxLayout>
#include <QDebug>

#include "qhexedit.h"

HexEditor::HexEditor(Project *project, QWidget *parent)
    : Editor(project, parent)
{
    QHBoxLayout *layout = new QHBoxLayout();
    layout->setMargin(0);

    _hexEditor = new QHexEdit();

    // style
    QString style = "\
    QFrame\
    {\
            background-color: #232629;\
            padding: 5px;\
            border-style: solid;\
            border: 1px solid #76797C;\
            border-radius: 2px;\
            color: #eff0f1;\
    }";
    _hexEditor->setStyleSheet(style);
    _hexEditor->setAddressAreaColor(0x31363B);
    _hexEditor->setSelectionColor(0x9D550F);
    _hexEditor->setHighlightingColor(0xFFE792);

    layout->addWidget(_hexEditor);
    setLayout(layout);
}

bool HexEditor::isModified() const
{
    return false;
}

HexEditor::SearchCaps HexEditor::searchCap() const
{
    return NoSearch;
}

int HexEditor::openFileData(const QString &filePath)
{
    _file.setFileName(filePath);
    _hexEditor->setData(_file);
    setFilePath(filePath);
    return 0;
}

int HexEditor::saveFileData(const QString &filePath)
{
    return 0;
}
