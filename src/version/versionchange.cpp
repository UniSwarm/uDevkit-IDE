#include "versionchange.h"

VersionChange::VersionChange()
{

}

int VersionChange::lineOld() const
{
    return _lineOld;
}

void VersionChange::setLineOld(int line)
{
    _lineOld = line;
}

int VersionChange::lineNew() const
{
    return _lineNew;
}

void VersionChange::setLineNew(int line)
{
    _lineNew = line;
}

const QStringList &VersionChange::addedLines() const
{
    return _addedLines;
}

void VersionChange::addAddedLine(const QString &line)
{
    _addedLines.append(line);
}

const QStringList &VersionChange::removedLines() const
{
    return _removedLines;
}

void VersionChange::addRemovedLine(const QString &line)
{
    _removedLines.append(line);
}
