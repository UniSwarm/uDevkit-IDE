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

#include "hexfileeditor.h"

#include <QApplication>
#include <QInputDialog>

#include "hexfile.h"
#include "qhexedit.h"

HexFileEditor::HexFileEditor(Project *project, QWidget *parent)
    : HexEditor(project, parent)
{
}

HexFileEditor::~HexFileEditor()
{
}

int HexFileEditor::openFileData(const QString &filePath)
{
    qint64 localCursorPosition = _hexEditor->cursorPosition();
    _file.setFileName(filePath);
    class HexFile hexFile(filePath);
    hexFile.read();
    _hexEditor->setData(hexFile.prog());
    _modified = false;
    emit modified(isModified());
    setFilePath(filePath);
    _hexEditor->setCursorPosition(localCursorPosition);
    return 0;
}

int HexFileEditor::saveFileData(const QString &filePath)
{
    QFile file(filePath + ".bin");
    file.open(QIODevice::WriteOnly);
    class HexFile hexFile(this->filePath());
    hexFile.read();
    int startMem = QInputDialog::getInt(this, tr("startMem"), tr("startMemAddr"), 0, 0, 0x80000);
    int lenghtMem = QInputDialog::getInt(this, tr("lenghtMem"), tr("lenghtMem"), 0x400, 0, 0x80000);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    file.write(hexFile.prog().mid(startMem, lenghtMem));
    QApplication::restoreOverrideCursor();
    _modified = false;
    emit modified(false);
    return 0;
}
