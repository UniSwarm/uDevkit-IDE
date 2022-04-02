/**
 ** This file is part of the uDevkit-IDE project.
 ** Copyright 2017-2021 UniSwarm
 **
 ** This program is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU General Public License as published by
 ** the Free Software Foundation, either version 3 of the License, or
 ** (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 ** GNU General Public License for more details.
 **
 ** You should have received a copy of the GNU General Public License
 ** along with this program. If not, see <http://www.gnu.org/licenses/>.
 **/

#include "project.h"

#include "make/makeparser.h"
#include "version/gitversioncontrol.h"

#include <QDebug>

Project::Project(const QString &path)
{
    _projectItemModel = new ProjectItemModel(this);
    _versionControl = new GitVersionControl();
    connect(_versionControl, &GitVersionControl::newModifiedFiles, _projectItemModel, &ProjectItemModel::filesUpdated);
    connect(_versionControl, &GitVersionControl::newValidatedFiles, _projectItemModel, &ProjectItemModel::filesUpdated);

    _make = new MakeParser();
    connect(_make, &MakeParser::sourceFilesAdded, this, &Project::newSource);
    connect(_make, &MakeParser::sourceFilesRemoved, this, &Project::oldSource);

    if (!path.isEmpty())
    {
        setRootPath(path);
    }
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
    _projectItemModel->clear();
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

void Project::addOpenedFiles(const QSet<QString> &openedFiles)
{
    foreach (QString file, openedFiles) _openedFiles.insert(file);
    _projectItemModel->filesUpdated(openedFiles);
    _projectItemModel->addOtherSource(openedFiles);
}

void Project::removeOpenedFiles(const QSet<QString> &openedFiles)
{
    foreach (QString file, openedFiles) _openedFiles.remove(file);
    _projectItemModel->filesUpdated(openedFiles);
    _projectItemModel->removeOtherSource(openedFiles);
}

MakeParser *Project::make() const
{
    return _make;
}

void Project::newSource(const QSet<QString> &sources)
{
    _projectItemModel->addExternalSource(sources);
}

void Project::oldSource(const QSet<QString> &sources)
{
    _projectItemModel->removeExternalSource(sources);
}
