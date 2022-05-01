/**
 ** This file is part of the uDevkit-IDE project.
 ** Copyright 2017-2021 UniSwarm
 **
 ** This program is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU General Public License as published by
 ** the Free Software Foundation, either version 3 of the License, or
 ** (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 ** GNU General Public License for more details.
 **
 ** You should have received a copy of the GNU General Public License
 ** along with this program. If not, see <http://www.gnu.org/licenses/>.
 **/

#include "hexeditor.h"

#include <QApplication>
#include <QBoxLayout>
#include <QDebug>
#include <QKeyEvent>
#include <QSaveFile>

#include <cmath>

#include "document/buffer/qmemorybuffer.h"
#include "qhexview.h"

HexEditor::HexEditor(Project *project, QWidget *parent)
    : Editor(project, parent)
{
    QHBoxLayout *layout = new QHBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);

    _hexEditor = new QHexView();
    _modified = false;

    // style
    /*QString style = "\
    QFrame\
    {\
            background-color: #19232D;\
            padding: 5px;\
            border-style: solid;\
            border: 1px solid #76797C;\
            border-radius: 2px;\
            color: #eff0f1;\
    }";
    _hexEditor->setStyleSheet(style);*/
    //_hexEditor->setAddressAreaColor(0x31363B);
    //_hexEditor->setSelectionColor(0x9D550F);
    //_hexEditor->setHighlightingColor(0xFFE792);

    layout->addWidget(_hexEditor);
    setLayout(layout);
}

HexEditor::~HexEditor()
{
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
    emit undoAvailable(true);
    emit modified(isModified());
}

void HexEditor::updatePos()
{
    QString status;
    int64_t maxAdress = _hexEditor->document()->baseAddress() + _hexEditor->document()->length();
    int addrWidth = ceil(log2(maxAdress) / 4.0);
    QString addr = QString::number(_hexEditor->document()->cursor()->position().offset(), 16).rightJustified(addrWidth, '0').toUpper();
    status.append(tr("addr: 0x%1 size: %2 ").arg(addr).arg(_hexEditor->document()->length()));
    emit statusChanged(status);
}

int HexEditor::openFileData(const QString &filePath)
{
    qint64 localCursorPosition = _hexEditor->document()->cursor()->position().offset();

    QHexDocument *document = QHexDocument::fromFile<QMemoryBuffer>(filePath);
    _hexEditor->setDocument(document);
    connect(_hexEditor->document(), &QHexDocument::documentChanged, this, &HexEditor::modificationAppend);
    connect(_hexEditor->document()->cursor(), &QHexCursor::positionChanged, this, &HexEditor::updatePos);

    _hexEditor->document()->cursor()->moveTo(localCursorPosition);

    _modified = false;
    emit modified(isModified());
    setFilePath(filePath);
    return 0;
}

int HexEditor::saveFileData(const QString &filePath)
{
    QString path = filePath.isEmpty() ? _filePath : filePath;
    QString tmpFileName = path + ".~tmp";

    QApplication::setOverrideCursor(Qt::WaitCursor);
    QFile file(tmpFileName);
    bool ok = _hexEditor->document()->saveTo(&file);
    if (QFile::exists(path))
    {
        ok = QFile::remove(path);
    }
    if (ok)
    {
        file.setFileName(tmpFileName);
        ok = file.copy(path);
        if (ok)
        {
            ok = QFile::remove(tmpFileName);
        }
    }
    QApplication::restoreOverrideCursor();
    _modified = false;
    emit modified(false);

    return ok ? 0 : -1;
}

void HexEditor::giveFocus()
{
    updatePos();
    emit copyAvailable(true);
}

void HexEditor::undoCommand()
{
    _hexEditor->document()->undo();
}

void HexEditor::redoCommand()
{
    _hexEditor->document()->redo();
}

void HexEditor::cutCommand()
{
    _hexEditor->document()->cut();
}

void HexEditor::copyCommand()
{
    _hexEditor->document()->copy();
}

void HexEditor::pasteCommand()
{
    _hexEditor->document()->paste();
}
