#include "fileversionchange.h"

FileVersionChange::FileVersionChange()
{
}

QList<VersionChange> &FileVersionChange::changes()
{
    return _changes;
}
