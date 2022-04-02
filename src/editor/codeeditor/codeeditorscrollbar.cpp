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

#include "codeeditorscrollbar.h"

#include <QApplication>
#include <QDebug>
#include <QPainter>
#include <QStyle>
#include <QStyleOptionComplex>

#include "edbee/models/textdocument.h"

CodeEditorScrollBar::CodeEditorScrollBar(QWidget *parent)
    : QScrollBar(parent)
{
    _fileChange = nullptr;
    _textRender = nullptr;
}

CodeEditorScrollBar::CodeEditorScrollBar(Qt::Orientation orientation, QWidget *parent)
    : QScrollBar(orientation, parent)
{
    _fileChange = nullptr;
    _textRender = nullptr;
}

void CodeEditorScrollBar::paintEvent(QPaintEvent *event)
{
    if (_fileChange == nullptr)
    {
        return;
    }

    QScrollBar::paintEvent(event);

    QStyleOptionSlider opt;
    initStyleOption(&opt);
    opt.subControls = QStyle::SC_All;
    int scrollBarExtent = style()->subControlRect(QStyle::CC_ScrollBar, &opt, QStyle::SC_ScrollBarAddLine, this).height();

    QPainter painter(this);

    int lineCount = qMax(_textRender->textDocument()->lineCount() - 1, 1);
    int barWidth = width() / 2;
    int offset = (width() - barWidth) / 2;
    foreach (VersionChange *change, _fileChange->changes())
    {
        int top = (change->lineNew() - 2) * (height() - scrollBarExtent * 2) / lineCount + scrollBarExtent;
        int barHeight = change->addedLines().count() * (height() - scrollBarExtent * 2) / lineCount + 2;
        QColor barColor;
        if (change->addedLines().count() < change->removedLines().count())
        {
            if (change->addedLines().count() == 0)
            {
                barColor.setRgb(255, 0, 0, 160);
            }
            else
            {
                barColor.setRgb(0, 255, 0, 100);
            }
        }
        else
        {
            if (change->addedLines().count() > change->removedLines().count())
            {
                barColor.setRgb(0, 255, 0, 100);
            }
            else
            {
                barColor.setRgb(0, 60, 255, 100);
            }
        }

        painter.setBrush(barColor);
        painter.fillRect(offset, top, barWidth, barHeight, QBrush(barColor));
    }
}

edbee::TextRenderer *CodeEditorScrollBar::textRender() const
{
    return _textRender;
}

void CodeEditorScrollBar::setTextRender(edbee::TextRenderer *textRender)
{
    _textRender = textRender;
}

FileVersionChange *CodeEditorScrollBar::fileChange() const
{
    return _fileChange;
}

void CodeEditorScrollBar::setFileChange(FileVersionChange *fileChange)
{
    _fileChange = fileChange;
    update();
}
