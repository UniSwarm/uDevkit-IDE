#ifndef FILEVERSIONCHANGE_H
#define FILEVERSIONCHANGE_H

#include <QList>

#include "versionchange.h"

class FileVersionChange
{
public:
    FileVersionChange();

    QList<VersionChange> &changes();
    QList<VersionChange> changesForRange(int lineStart, int lineEnd) const;

protected:
    QList<VersionChange> _changes;
};

#endif // FILEVERSIONCHANGE_H
