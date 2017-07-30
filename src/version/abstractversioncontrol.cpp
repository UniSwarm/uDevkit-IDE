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

bool AbstractVersionControl::isDirTracked(const QString &filePath) const
{
    if (!isValid())
        return true;
    QSetIterator<QString> i(_trackedFiles);
    while (i.hasNext())
        if (i.next().startsWith(filePath))
            return true;
    return false;
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

bool AbstractVersionControl::isFileTracked(const QString &filePath) const
{
    if (!isValid())
        return true;
    return _trackedFiles.contains(filePath);
}

bool AbstractVersionControl::isFileValidated(const QString &filePath) const
{
    return _validatedFiles.contains(filePath);
}

const QSet<QString> &AbstractVersionControl::trackedFiles()
{
    return _trackedFiles;
}

const QSet<QString> &AbstractVersionControl::modifiedFiles()
{
    return _modifiedFiles;
}

const QSet<QString> &AbstractVersionControl::validatedFiles()
{
    return _validatedFiles;
}

AbstractVersionControl::isValid() const
{
    return true;
}

void AbstractVersionControl::analysePath()
{
}
