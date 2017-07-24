#include "abstractversioncontrol.h"

#include <QDir>
#include <QDebug>

AbstractVersionControl::AbstractVersionControl(QObject *parent) : QObject(parent)
{
}

void AbstractVersionControl::setPath(const QString &path)
{
    _path = QDir::cleanPath(path);
    analysePath();
}

bool AbstractVersionControl::isFileModified(const QString &filePath) const
{
    return _modifiedFiles.contains(filePath);
}

bool AbstractVersionControl::isDirModified(const QString &filePath) const
{
    QSetIterator<QString> i(_modifiedFiles);
    while (i.hasNext())
        if (i.next().startsWith(filePath))
            return true;
    return false;
}

const QSet<QString> &AbstractVersionControl::trackedFiles()
{
    return _trackedFiles;
}

const QSet<QString> &AbstractVersionControl::modifiedFiles()
{
    return _modifiedFiles;
}

void AbstractVersionControl::analysePath()
{
}
