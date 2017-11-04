#include "codeeditormargindelegate.h"

#include <QPainter>
#include <QDebug>

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

void CodeEditorMarginDelegate::renderBefore(QPainter *painter, int startLine, int endLine, int width)
{
    QList<VersionChange> changesForRange = _fileChange.changesForRange(startLine, endLine);
    foreach (const VersionChange &change, changesForRange)
    {
        int start = marginComponent()->renderer()->yPosForLine(change.lineNew()-1);
        int end = marginComponent()->renderer()->yPosForLine(change.lineNew()-1 + change.addedLines().count());
        if (change.addedLines().count() > change.removedLines().count())
            painter->setPen(QPen(Qt::green, 2));
        else if (change.addedLines().count() < change.removedLines().count())
            painter->setPen(QPen(Qt::red, 2));
        else
            painter->setPen(QPen(Qt::blue, 2));
        painter->drawLine(width-2, start, width-2, end);
    }
}
