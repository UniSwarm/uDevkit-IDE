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

#include "hexfileeditor.h"

#include <QApplication>
#include <QInputDialog>

#include <cmath>

#include "hexfile.h"

#include "qhexview.h"
#include "document/buffer/qmemorybuffer.h"

HexFileEditor::HexFileEditor(Project *project, QWidget *parent)
    : HexEditor(project, parent)
{
}

HexFileEditor::~HexFileEditor()
{
}

int HexFileEditor::openFileData(const QString &filePath)
{
    class HexFile hexFile(filePath);
    hexFile.read();

    qint64 localCursorPosition = _hexEditor->document()->cursor()->position().offset();

    QHexDocument *document = QHexDocument::fromMemory<QMemoryBuffer>(hexFile.prog());
    _hexEditor->setDocument(document);
    connect(_hexEditor->document(), &QHexDocument::documentChanged, this, &HexFileEditor::modificationAppend);
    connect(_hexEditor->document()->cursor(), &QHexCursor::positionChanged, this, &HexFileEditor::updatePos);

    _hexEditor->document()->cursor()->moveTo(localCursorPosition);

    _modified = false;
    emit modified(isModified());
    setFilePath(filePath);
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

void HexFileEditor::updatePos()
{
    QString status;
    int64_t maxAdress = _hexEditor->document()->baseAddress() + _hexEditor->document()->length();
    int addrWidth = ceil(log2(maxAdress) / 4.0);
    QString addr = QString::number(_hexEditor->document()->cursor()->position().offset(), 16).rightJustified(addrWidth, '0').toUpper();
    status.append(tr("word addr: 0x%1 size: %2 ").arg(addr).arg(_hexEditor->document()->length()));
    emit statusChanged(status);
}
