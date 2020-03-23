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

#include "codeeditormargindelegate.h"

#include <QDebug>
#include <QMenu>
#include <QMouseEvent>
#include <QPainter>

#include "edbee/views/textrenderer.h"
#include "edbee/views/texttheme.h"

const int widthBar = 10;

CodeEditorMarginDelegate::CodeEditorMarginDelegate()
    : edbee::TextMarginComponentDelegate()
{
    _fileChange = Q_NULLPTR;
}

CodeEditorMarginDelegate::~CodeEditorMarginDelegate()
{
}

FileVersionChange *CodeEditorMarginDelegate::fileChange() const
{
    return _fileChange;
}

void CodeEditorMarginDelegate::setFileChange(FileVersionChange *fileChange)
{
    _fileChange = fileChange;
}

int CodeEditorMarginDelegate::widthBeforeLineNumber()
{
    return widthBar;
}

void CodeEditorMarginDelegate::renderAfter(QPainter *painter, int startLine, int endLine, int width)
{
    if (!_fileChange)
    {
        return;
    }

    QLinearGradient linearGrad(QPointF(0, 0), QPointF(widthBar, 0));
    linearGrad.setColorAt(1, QColor(0, 0, 0, 0));

    QList<VersionChange *> changesForRange = _fileChange->changesForRange(startLine + 1, endLine + 1);
    foreach (VersionChange *change, changesForRange)
    {
        int start = marginComponent()->renderer()->yPosForLine(change->lineNew() - 1);
        int end = marginComponent()->renderer()->yPosForLine(change->lineNew() - 1 + change->addedLines().count());
        if (change->addedLines().count() < change->removedLines().count())
        {
            int l = start;
            if (change->addedLines().count() == 0)
            {
                l = marginComponent()->renderer()->yPosForLine(change->lineNew());
            }
            else
            {
                linearGrad.setFinalStop(QPointF(widthBar, 0));
                linearGrad.setColorAt(0, QColor(0, 60, 180, 255)); // blue
                painter->fillRect(QRect(0, start, widthBar, end - start), QBrush(linearGrad));
            }
            linearGrad.setFinalStop(QPointF(width - 8, 0));
            linearGrad.setColorAt(0, QColor(255, 0, 0, 255)); // red
            painter->fillRect(QRect(0, l - 1, width - 8, 2), QBrush(linearGrad));
        }
        else
        {
            int l = end;
            if (change->addedLines().count() > change->removedLines().count())
            {
                if (!change->removedLines().isEmpty())
                {
                    l = marginComponent()->renderer()->yPosForLine(change->lineNew() + change->addedLines().count() - change->removedLines().count());
                }
                else
                {
                    l = start;
                }
                linearGrad.setFinalStop(QPointF(widthBar, 0));
                linearGrad.setColorAt(0, QColor(0, 128, 0, 255)); // green
                painter->fillRect(QRect(0, l, widthBar, end - l), QBrush(linearGrad));
            }
            if (l != start)
            {
                linearGrad.setFinalStop(QPointF(widthBar, 0));
                linearGrad.setColorAt(0, QColor(0, 60, 180, 255)); // blue
                painter->fillRect(QRect(0, start, widthBar, l - start), QBrush(linearGrad));
            }
        }
        if (change->isStaged())
        {
            painter->setPen(QColor(255, 128, 0, 200));
            painter->drawLine(0, start, 0, end);
        }
    }
}

void CodeEditorMarginDelegate::mousePressEvent(int line, QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
    {
        QList<VersionChange *> changesForRange = _fileChange->changesForRange(line + 1, line + 1);
        if (!changesForRange.isEmpty())
        {
            if (changesForRange.at(0)->removedLines().isEmpty())
            {
                return;
            }
            QMenu menu;
            foreach (QString line, changesForRange.at(0)->removedLines())
                menu.addAction(line);
            menu.exec(event->screenPos().toPoint());
        }
    }
    else
    {
        edbee::TextMarginComponentDelegate::mousePressEvent(line, event);
    }
}
