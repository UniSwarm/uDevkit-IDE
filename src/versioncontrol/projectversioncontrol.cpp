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

#include "projectversioncontrol.h"

#include "gitversioncontrol.h"

#include <QDebug>
#include <QDir>
#include <QFileInfo>

ProjectVersionControl::ProjectVersionControl(QObject *parent)
    : AbstractVersionControl(parent)
{
    _baseVersionControl = nullptr;
}

ProjectVersionControl::~ProjectVersionControl()
{
    qDeleteAll(_versionControlMap);
}

AbstractVersionControl *ProjectVersionControl::versionControl(const QString &filePath) const
{
    if (filePath.isEmpty())
    {
        return nullptr;
    }

    QString dirPath = QFileInfo(filePath).dir().absolutePath();
    AbstractVersionControl *versionControl = _versionControlMap[dirPath];
    if (versionControl != nullptr)
    {
        return versionControl;
    }

    QString root = findVersionRoot(dirPath);
    versionControl = _versionControlMap[root];
    if (versionControl != nullptr)
    {
        return versionControl;
    }

    return createVersionRoot(dirPath);
}

AbstractVersionControl *ProjectVersionControl::baseVersionControl() const
{
    return _baseVersionControl;
}

void ProjectVersionControl::addNewVersionControl(AbstractVersionControl *versionControl, const QString &path) const
{
    connect(versionControl, &AbstractVersionControl::newModifiedFiles, this, &ProjectVersionControl::processModifiedFiles);
    connect(versionControl, &AbstractVersionControl::newValidatedFiles, this, &ProjectVersionControl::processValidatedFiles);
    connect(versionControl, &AbstractVersionControl::fileModificationsAvailable, this, &ProjectVersionControl::processModifAvailable);
    versionControl->setPath(path);
    _versionControls.append(versionControl);
    _versionControlMap.insert(path, versionControl);
}

void ProjectVersionControl::processModifiedFiles(const QSet<QString> &modifiedFiles)
{
    _trackedFiles.clear();
    _validatedFiles.clear();
    _modifiedFiles.clear();
    for (AbstractVersionControl *version : _versionControls)
    {
        _trackedFiles.unite(version->trackedFiles());
        _validatedFiles.unite(version->validatedFiles());
        _modifiedFiles.unite(version->modifiedFiles());
    }

    emit newModifiedFiles(modifiedFiles);
}

void ProjectVersionControl::processValidatedFiles(const QSet<QString> &validatedFiles)
{
    _trackedFiles.clear();
    _validatedFiles.clear();
    _modifiedFiles.clear();
    for (AbstractVersionControl *version : _versionControls)
    {
        _trackedFiles.unite(version->trackedFiles());
        _validatedFiles.unite(version->validatedFiles());
        _modifiedFiles.unite(version->modifiedFiles());
    }
    emit newValidatedFiles(validatedFiles);
}

void ProjectVersionControl::processModifAvailable(const QString &file)
{
    emit fileModificationsAvailable(file);
}

QString ProjectVersionControl::findVersionRoot(const QString &path) const
{
    QDir dir(path);
    do
    {
        if (dir.exists(".git"))
        {
            return dir.absolutePath();
        }
    } while (dir.cdUp());
    return QString();
}

AbstractVersionControl *ProjectVersionControl::createVersionRoot(const QString &path) const
{
    AbstractVersionControl *versionControl = nullptr;

    QDir dir(path);
    do
    {
        if (dir.exists(".git"))
        {
            versionControl = new GitVersionControl();
            // qDebug() << "new git" << dir.absolutePath() << path;
            break;
        }
    } while (dir.cdUp());

    if (versionControl == nullptr)
    {
        versionControl = new AbstractVersionControl();
        // qDebug() << "new abstract version" << dir.absolutePath() << path;
    }
    addNewVersionControl(versionControl, dir.absolutePath());
    return versionControl;
}

QString ProjectVersionControl::versionControlName() const
{
    return "ProjectVersion";
}

void ProjectVersionControl::validFile(const QSet<QString> &filesPath)
{
    for (const QString &filePath : filesPath)
    {
        versionControl(filePath)->validFile({filePath});
    }
}

void ProjectVersionControl::inValidFile(const QSet<QString> &filesPath)
{
    for (const QString &filePath : filesPath)
    {
        versionControl(filePath)->inValidFile({filePath});
    }
}

void ProjectVersionControl::checkoutFile(const QSet<QString> &filesPath)
{
    for (const QString &filePath : filesPath)
    {
        versionControl(filePath)->checkoutFile({filePath});
    }
}

bool ProjectVersionControl::isValid() const
{
    return true;
}

void ProjectVersionControl::requestFileModifications(const QString &filePath)
{
    versionControl(filePath)->requestFileModifications(filePath);
}

void ProjectVersionControl::analysePath()
{
    qDeleteAll(_versionControls);
    _versionControls.clear();
    _versionControlMap.clear();

    _baseVersionControl = createVersionRoot(_path);
}

FileVersionChange ProjectVersionControl::fileModifications(const QString &filePath)
{
    return versionControl(filePath)->fileModifications(filePath);
}
