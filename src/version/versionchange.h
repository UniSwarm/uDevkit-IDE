#ifndef VERSIONCHANGE_H
#define VERSIONCHANGE_H

#include <QStringList>

class VersionChange
{
public:
    VersionChange();

    int lineOld() const;
    void setLineOld(int line);

    int lineNew() const;
    void setLineNew(int line);

    const QStringList &addedLines() const;
    void addAddedLine(const QString &line);

    const QStringList &removedLines() const;
    void addRemovedLine(const QString &line);

    void clear();
    bool isValid() const;

protected:
    int _lineOld;
    int _lineNew;
    QStringList _addedLines;
    QStringList _removedLines;
};

#endif // VERSIONCHANGE_H
