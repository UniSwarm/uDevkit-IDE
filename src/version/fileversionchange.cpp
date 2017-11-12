#include "fileversionchange.h"

#include <QDebug>

FileVersionChange::FileVersionChange()
{
}

FileVersionChange::FileVersionChange(const FileVersionChange &other)
{
    foreach (VersionChange *change, other._changes)
        _changes.append(new VersionChange(*change));
}

FileVersionChange::~FileVersionChange()
{
    qDeleteAll(_changes);
}

FileVersionChange &FileVersionChange::operator=(const FileVersionChange &other)
{
    if (this != &other)
    {
        qDeleteAll(_changes);
        _changes.clear();
        foreach (VersionChange *change, other._changes)
            _changes.append(new VersionChange(*change));
    }
    return *this;
}

QList<VersionChange *> &FileVersionChange::changes()
{
    return _changes;
}

QList<VersionChange *> FileVersionChange::changesForRange(int lineStart, int lineEnd) const
{
    QList<VersionChange *> changes;
    foreach (VersionChange *change, _changes)
    {
        if (change->lineNew() <= lineEnd && (change->lineNew() + change->addedLines().count()) > lineStart)
            changes.append(change);
    }
    return changes;
}

void FileVersionChange::insertChange(const VersionChange &change)
{
    // update all change after the current insert pos
    int lineOffset = change.addedLines().count() - change.removedLines().count();
    insertLines(change.lineNew(), lineOffset);

    QList<VersionChange *> actualChanges = changesForRange(change.lineNew(), change.lineNew());
    VersionChange *nchange;
    if (actualChanges.isEmpty()) // add a new change here
    {
        nchange = new VersionChange(change);
        _changes.append(nchange);
    }
    else // merge with already existing changes
    {
        nchange = actualChanges.first();
        /*for (int i=nchange->lineNew(); i<nchange->addedLines().count()-1; i++)
            nchange->addedLines().addAddedLine(nchange->addedLines().at(i));*/
        nchange->addedLines() = change.addedLines();
        // TODO complete merge
    }
    //qDebug()<<nchange->removedLines().count()<<nchange->addedLines().count();
    //qDebug()<<nchange->removedLines()<<nchange->addedLines();
}

void FileVersionChange::insertLines(int fromLine, int lineCount)
{
    foreach (VersionChange *change, _changes)
    {
        if (change->lineNew() > fromLine)
            change->setLineNew(change->lineNew()+lineCount);
    }
}

