#include "project.h"

#include "version/gitversioncontrol.h"
#include "make/makeparser.h"

#include <QDebug>

Project::Project(const QString &path) : QObject()
{
    _projectItemModel = new ProjectItemModel(this);
    //_projectItemModel->setFilter(QDir::AllEntries | QDir::Hidden | QDir::NoDotAndDotDot);
    _versionControl = new GitVersionControl();
    connect(_versionControl, &GitVersionControl::newModifiedFiles,
            _projectItemModel, &ProjectItemModel::filesUpdated);
    connect(_versionControl, &GitVersionControl::newValidatedFiles,
            _projectItemModel, &ProjectItemModel::filesUpdated);

    _make = new MakeParser();
    connect(_make, &MakeParser::sourceFilesAdded, this, &Project::newSource);
    connect(_make, &MakeParser::sourceFilesRemoved, this, &Project::oldSource);

    if (!path.isEmpty())
        setRootPath(path);
}

Project::~Project()
{
    delete _make;
    delete _projectItemModel;
    delete _versionControl;
}

const QDir &Project::rootDir() const
{
    return _rootDir;
}

QString Project::rootPath() const
{
    return _rootDir.canonicalPath();
}

void Project::setRootPath(const QString &path)
{
    _rootDir.setPath(QDir::cleanPath(path));

    _make->setPath(rootPath());
    _versionControl->setPath(rootPath());
    _projectItemModel->addRealDirItem(rootPath());
    emit rootPathChanged();
}

AbstractVersionControl *Project::versionControl() const
{
    return _versionControl;
}

ProjectItemModel *Project::projectItemModel() const
{
    return _projectItemModel;
}

bool Project::isOpenedFile(const QString &path) const
{
    return _openedFiles.contains(path);
}

const QSet<QString> &Project::openedFiles() const
{
    return _openedFiles;
}

void Project::addOpenedFiles(QSet<QString> openedFiles)
{
    foreach (QString file, openedFiles)
        _openedFiles.insert(file);
    _projectItemModel->filesUpdated(openedFiles);
}

void Project::removeOpenedFiles(QSet<QString> openedFiles)
{
    foreach (QString file, openedFiles)
        _openedFiles.remove(file);
    _projectItemModel->filesUpdated(openedFiles);
}

MakeParser *Project::make() const
{
    return _make;
}

void Project::newSource(QSet<QString> sources)
{
    _projectItemModel->addExternalSource(sources);
}

void Project::oldSource(QSet<QString> sources)
{
    _projectItemModel->removeExternalSource(sources);
}
