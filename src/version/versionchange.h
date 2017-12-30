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

    QStringList &addedLines();
    const QStringList &addedLines() const;
    void addAddedLine(const QString &line);

    QStringList &removedLines();
    const QStringList &removedLines() const;
    void addRemovedLine(const QString &line);

    void clear();
    bool isValid() const;

    bool isStaged() const;
    void setStaged(bool staged);

protected:
    int _lineOld;
    int _lineNew;
    QStringList _addedLines;
    QStringList _removedLines;
    bool _staged;
};

#endif // VERSIONCHANGE_H
