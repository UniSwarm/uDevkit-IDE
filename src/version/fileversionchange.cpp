#include "fileversionchange.h"

#include <QDebug>

FileVersionChange::FileVersionChange()
{
}

QList<VersionChange> &FileVersionChange::changes()
{
    return _changes;
}

QList<VersionChange> FileVersionChange::changesForRange(int lineStart, int lineEnd) const
{
    QList<VersionChange> changes;
    foreach (const VersionChange &change, _changes)
    {
        if (change.lineNew() <= lineEnd + 1 && (change.lineNew() + change.addedLines().count()) > lineStart)
            changes.append(change);
    }
    return changes;
}
