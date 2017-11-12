#ifndef FILEVERSIONCHANGE_H
#define FILEVERSIONCHANGE_H

#include <QList>

#include "versionchange.h"

class FileVersionChange
{
public:
    FileVersionChange();
    FileVersionChange(const FileVersionChange &other);
    ~FileVersionChange();
    FileVersionChange &operator=(const FileVersionChange &other);

    QList<VersionChange *> &changes();
    QList<VersionChange *> changesForRange(int lineStart, int lineEnd) const;

    void insertChange(const VersionChange &change);

protected:
    void insertLines(int fromLine, int lineCount);
    QList<VersionChange *> _changes;
};

#endif // FILEVERSIONCHANGE_H
