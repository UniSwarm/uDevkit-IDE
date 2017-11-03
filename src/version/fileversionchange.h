#ifndef FILEVERSIONCHANGE_H
#define FILEVERSIONCHANGE_H

#include <QList>

#include "versionchange.h"

class FileVersionChange
{
public:
    FileVersionChange();

    QList<VersionChange> &changes();

protected:
    QList<VersionChange> _changes;
};

#endif // FILEVERSIONCHANGE_H
