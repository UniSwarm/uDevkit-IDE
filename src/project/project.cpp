#include "project.h"

#include "version/gitversioncontrol.h"

Project::Project(const QString &path) : QObject()
{
    _fileItemModel = new FileProjectItemModel(this);
    _fileItemModel->setFilter(QDir::AllEntries | QDir::Hidden | QDir::NoDotAndDotDot);
    _versionControl = new GitVersionControl();
    connect(_versionControl, &GitVersionControl::newModifiedFiles,
            _fileItemModel, &FileProjectItemModel::filesUpdated);
    connect(_versionControl, &GitVersionControl::newValidatedFiles,
            _fileItemModel, &FileProjectItemModel::filesUpdated);

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
    _versionControl->setPath(path);
    _fileItemModel->setRootPath(rootPath());
    emit rootPathChanged();
}

AbstractVersionControl *Project::versionControl() const
{
    return _versionControl;
}

FileProjectItemModel *Project::fileItemModel() const
{
    return _fileItemModel;
}
