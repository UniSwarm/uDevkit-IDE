#include "hexeditor.h"

#include <QApplication>
#include <QBoxLayout>
#include <QDebug>
#include <QSaveFile>

#include "qhexedit.h"

HexEditor::HexEditor(Project *project, QWidget *parent)
    : Editor(project, parent)
{
    QHBoxLayout *layout = new QHBoxLayout();
    layout->setMargin(0);

    _hexEditor = new QHexEdit();
    _modified = false;
    connect(_hexEditor, &QHexEdit::dataChanged, this, &HexEditor::modificationAppend);

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
    return _modified;
}

HexEditor::SearchCaps HexEditor::searchCap() const
{
    return NoSearch;
}

void HexEditor::modificationAppend()
{
    _modified = true;
    emit modified(isModified());
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
    QString path = filePath.isEmpty() ? _filePath : filePath;
    QString tmpFileName = path + ".~tmp";

    QApplication::setOverrideCursor(Qt::WaitCursor);
    QFile file(tmpFileName);
    bool ok = _hexEditor->write(file);
    if (QFile::exists(path))
        ok = QFile::remove(path);
    if (ok)
    {
        file.setFileName(tmpFileName);
        ok = file.copy(path);
        if (ok)
            ok = QFile::remove(tmpFileName);
    }
    QApplication::restoreOverrideCursor();
    _modified = false;
    emit modified(false);

    return ok ? 0 : -1;
}
