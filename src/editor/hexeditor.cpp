/**
 ** This file is part of the uDevkit-IDE project.
 ** Copyright 2017-2020 UniSwarm
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

#include "qhexedit.h"

HexEditor::HexEditor(Project *project, QWidget *parent)
    : Editor(project, parent)
{
    QHBoxLayout *layout = new QHBoxLayout();
    layout->setMargin(0);

    _hexEditor = new QHexEdit();
    _modified = false;
    connect(_hexEditor, &QHexEdit::dataChanged, this, &HexEditor::modificationAppend);
    connect(_hexEditor, &QHexEdit::currentAddressChanged, this, &HexEditor::updatePos);

    // style
    QString style = "\
    QFrame\
    {\
            background-color: #19232D;\
            padding: 5px;\
            border-style: solid;\
            border: 1px solid #76797C;\
            border-radius: 2px;\
            color: #eff0f1;\
            border: none;\
    }";
    _hexEditor->setStyleSheet(style);
    _hexEditor->setAddressAreaColor(0x31363B);
    _hexEditor->setSelectionColor(0x9D550F);
    _hexEditor->setHighlightingColor(0xFFE792);

    layout->addWidget(_hexEditor);
    setLayout(layout);
}

HexEditor::~HexEditor()
{
    _file.close();
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
    QString addr = QString::number(_hexEditor->cursorPosition(), 16).rightJustified(_hexEditor->addressWidth(), '0');
    status.append(tr("addr: 0x%1 size: %2 ").arg(addr).arg(_hexEditor->data().size()));
    emit statusChanged(status);
}

int HexEditor::openFileData(const QString &filePath)
{
    _file.setFileName(filePath);
    _hexEditor->setData(_file);
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
    bool ok = _hexEditor->write(file);
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
    _hexEditor->undo();
}

void HexEditor::redoCommand()
{
    _hexEditor->redo();
}

void HexEditor::cutCommand()
{
    QKeyEvent *eve1 = new QKeyEvent(QEvent::KeyPress, Qt::Key_X, Qt::ControlModifier);
    QKeyEvent *eve2 = new QKeyEvent(QEvent::KeyRelease, Qt::Key_X, Qt::ControlModifier);

    qApp->postEvent(_hexEditor, static_cast<QEvent *>(eve1));
    qApp->postEvent(_hexEditor, static_cast<QEvent *>(eve2));
}

void HexEditor::copyCommand()
{
    QKeyEvent *eve1 = new QKeyEvent(QEvent::KeyPress, Qt::Key_C, Qt::ControlModifier);
    QKeyEvent *eve2 = new QKeyEvent(QEvent::KeyRelease, Qt::Key_C, Qt::ControlModifier);

    qApp->postEvent(_hexEditor, static_cast<QEvent *>(eve1));
    qApp->postEvent(_hexEditor, static_cast<QEvent *>(eve2));
}

void HexEditor::pasteCommand()
{
    QKeyEvent *eve1 = new QKeyEvent(QEvent::KeyPress, Qt::Key_V, Qt::ControlModifier);
    QKeyEvent *eve2 = new QKeyEvent(QEvent::KeyRelease, Qt::Key_V, Qt::ControlModifier);

    qApp->postEvent(_hexEditor, static_cast<QEvent *>(eve1));
    qApp->postEvent(_hexEditor, static_cast<QEvent *>(eve2));
}
