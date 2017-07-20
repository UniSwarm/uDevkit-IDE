#include "abstractversioncontrol.h"

#include <QDir>

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
