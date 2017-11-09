#include "codeeditormargindelegate.h"

#include <QPainter>
#include <QDebug>
#include <QMouseEvent>
#include <QMenu>

#include "edbee/views/textrenderer.h"
#include "edbee/views/texttheme.h"

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
    return 10;
}

void CodeEditorMarginDelegate::renderAfter(QPainter *painter, int startLine, int endLine, int width)
{
    QLinearGradient linearGrad(QPointF(0, 0), QPointF(10, 0));
    linearGrad.setColorAt(1, marginComponent()->renderer()->theme()->backgroundColor());

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
                linearGrad.setFinalStop(QPointF(14, 0));
                linearGrad.setColorAt(0, Qt::green);
                painter->fillRect(QRect(0, start, 14, end-start), QBrush(linearGrad));
            }
            linearGrad.setFinalStop(QPointF(width-8, 0));
            linearGrad.setColorAt(0, Qt::red);
            painter->fillRect(QRect(0, l, width-8, 2), QBrush(linearGrad));
        }
        else
        {
            int l = end;
            if (change.addedLines().count() > change.removedLines().count())
            {
                if (!change.removedLines().isEmpty())
                    l = marginComponent()->renderer()->yPosForLine(change.lineNew() + change.addedLines().count() - change.removedLines().count());
                else
                    l = start;
                linearGrad.setFinalStop(QPointF(14, 0));
                linearGrad.setColorAt(0, Qt::green);
                painter->fillRect(QRect(0, l, 14, end-l), QBrush(linearGrad));
            }
            if (l != start)
            {
                linearGrad.setFinalStop(QPointF(14, 0));
                linearGrad.setColorAt(0, Qt::blue);
                painter->fillRect(QRect(0, start, 14, l-start), QBrush(linearGrad));
            }
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
