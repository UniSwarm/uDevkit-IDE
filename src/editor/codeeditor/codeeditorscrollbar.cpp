#include "codeeditorscrollbar.h"

#include <QPainter>
#include <QDebug>
#include <QStyle>
#include <QApplication>
#include <QStyleOptionComplex>

#include "edbee/models/textdocument.h"

CodeEditorScrollBar::CodeEditorScrollBar(QWidget *parent)
    : QScrollBar(parent)
{
}

CodeEditorScrollBar::CodeEditorScrollBar(Qt::Orientation orientation, QWidget *parent)
    : QScrollBar(orientation, parent)
{
}

void CodeEditorScrollBar::paintEvent(QPaintEvent *event)
{
    QScrollBar::paintEvent(event);

    QStyleOptionSlider opt;
    initStyleOption(&opt);
    opt.subControls = QStyle::SC_All;
    int scrollBarExtent = style()->subControlRect(QStyle::CC_ScrollBar, &opt, QStyle::SC_ScrollBarAddLine, this).height();

    QPainter painter(this);

    int lineCount = qMax(_textRender->textDocument()->lineCount() - 1, 1);
    int barWidth = width() / 2;
    int offset = (width() - barWidth) / 2;
    foreach (const VersionChange &change, _fileChange.changes())
    {
        int top = (change.lineNew() - 2) * (height() - scrollBarExtent * 2) / lineCount + scrollBarExtent;
        int barHeight = change.addedLines().count() * (height() - scrollBarExtent * 2) / lineCount + 2;
        QColor barColor;
        if (change.addedLines().count() < change.removedLines().count())
        {
            if (change.addedLines().count() == 0)
                barColor.setRgb(255, 0, 0, 160);
            else
                barColor.setRgb(0, 255, 0, 100);
        }
        else
        {
            if (change.addedLines().count() > change.removedLines().count())
                barColor.setRgb(0, 255, 0, 100);
            else
                barColor.setRgb(0, 0, 255, 100);
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

FileVersionChange CodeEditorScrollBar::fileChange() const
{
    return _fileChange;
}

void CodeEditorScrollBar::setFileChange(const FileVersionChange &fileChange)
{
    _fileChange = fileChange;
    update();
}
