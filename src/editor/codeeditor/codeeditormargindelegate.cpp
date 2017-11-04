#include "codeeditormargindelegate.h"

#include <QPainter>
#include <QDebug>
#include <QMouseEvent>
#include <QMenu>

#include "edbee/views/textrenderer.h"

CodeEditorMarginDelegate::CodeEditorMarginDelegate()
    : edbee::TextMarginComponentDelegate()
{
}

CodeEditorMarginDelegate::~CodeEditorMarginDelegate()
{
}

const FileVersionChange &CodeEditorMarginDelegate::fileChange() const
{
    return _fileChange;
}

void CodeEditorMarginDelegate::setFileChange(const FileVersionChange &fileChange)
{
    _fileChange = fileChange;
}

int CodeEditorMarginDelegate::widthBeforeLineNumber()
{
    return 5;
}

void CodeEditorMarginDelegate::renderAfter(QPainter *painter, int startLine, int endLine, int width)
{
    QList<VersionChange> changesForRange = _fileChange.changesForRange(startLine, endLine);
    foreach (const VersionChange &change, changesForRange)
    {
        int start = marginComponent()->renderer()->yPosForLine(change.lineNew()-1);
        int end = marginComponent()->renderer()->yPosForLine(change.lineNew()-1 + change.addedLines().count());
        if (change.addedLines().count() < change.removedLines().count())
        {
            int l = start;
            if (change.addedLines().count() == 0)
            {
                l = marginComponent()->renderer()->yPosForLine(change.lineNew());
            }
            else
            {
                painter->setPen(QPen(Qt::green, 2));
                painter->drawLine(width-6, start+1, width-6, end-1);
            }
            painter->setPen(QPen(Qt::red, 2));
            painter->drawLine(6, l+1, width-6, l+1);
        }
        else
        {
            int l = end;
            if (change.addedLines().count() > change.removedLines().count())
            {
                l = marginComponent()->renderer()->yPosForLine(change.lineNew() + change.addedLines().count() - change.removedLines().count());
                painter->setPen(QPen(Qt::green, 2));
                painter->drawLine(width-6, l+1, width-6, end-1);
            }
            painter->setPen(QPen(Qt::blue, 2));
            painter->drawLine(width-6, start+1, width-6, l-1);
        }
    }
}

void CodeEditorMarginDelegate::mousePressEvent(int line, QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
    {
        int line = marginComponent()->renderer()->lineIndexForYpos(event->y()) + 1;
        QList<VersionChange> changesForRange = _fileChange.changesForRange(line, line);
        if (!changesForRange.isEmpty())
        {
            QMenu menu;
            foreach (QString line, changesForRange.at(0).removedLines())
                menu.addAction(line);
            menu.exec(event->screenPos().toPoint());
        }
    }
    else
        edbee::TextMarginComponentDelegate::mousePressEvent(line, event);
}
