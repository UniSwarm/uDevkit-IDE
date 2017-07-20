#include "project.h"

#include "version/gitversioncontrol.h"

Project::Project(const QString &path) : QObject()
{
    if (!path.isEmpty())
        setRootPath(path);
}

const QDir &Project::rootDir() const
{
    return _rootDir;
}

QString Project::rootPath() const
{
    return _rootDir.path();
}

void Project::setRootPath(const QString &path)
{
    _rootDir.setPath(QDir::cleanPath(path));
    _versionControl = new GitVersionControl();
    _versionControl->setPath(path);
}

AbstractVersionControl *Project::versionControl() const
{
    return _versionControl;
}
