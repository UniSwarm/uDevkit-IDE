/**
 ** This file is part of the uDevkit-IDE project.
 ** Copyright 2017-2020 UniSwarm
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

#include "projectitem.h"

#include "project.h"
#include "projectitemmodel.h"

#include <QDebug>
#include <QDirIterator>
#include <QFont>
#include <QTimer>

ProjectItem::ProjectItem(Project *project, const QString path, Type type, ProjectItemModel *model)
    : QObject(Q_NULLPTR)
    , _type(type)
    , _info(path, project)
    , _model(model)
{
    _parentItem = Q_NULLPTR;

    _watcher = Q_NULLPTR;
    if (type == RealDir)
    {
        _watcher = new QFileSystemWatcher();
        _watcher->addPath(QDir::cleanPath(path) + "/");
        connect(_watcher, &QFileSystemWatcher::directoryChanged, this, &ProjectItem::updateModif);
        connect(_watcher, &QFileSystemWatcher::fileChanged, this, &ProjectItem::updateModif);

        QTimer::singleShot(0, this, SLOT(updateModif()));
        // updateModif();
    }

    if (type == IndividualFile)
    {
        _watcher = new QFileSystemWatcher();
        _watcher->addPath(QDir::cleanPath(path));
        connect(_watcher, &QFileSystemWatcher::fileChanged, this, &ProjectItem::updateModif);
    }
}

ProjectItem::~ProjectItem()
{
    qDeleteAll(_childrens);
    delete _watcher;
}

int ProjectItem::count() const
{
    return _childrens.count();
}

ProjectItem *ProjectItem::child(int row) const
{
    return _childrens.at(row);
}

ProjectItem *ProjectItem::child(const QString &name) const
{
    QHash<QString, ProjectItem *>::const_iterator i = _childrensMap.find(name);
    if (i != _childrensMap.end())
    {
        return *i;
    }
    return Q_NULLPTR;
}

int ProjectItem::row() const
{
    if (_parentItem)
    {
        return _parentItem->_childrens.indexOf(const_cast<ProjectItem *>(this));
    }

    return 0;
}

void ProjectItem::addChild(ProjectItem *child)
{
    child->_parentItem = this;
    _childrens.append(child);
    _childrensMap.insert(child->fileName(), child);
}

void ProjectItem::removeChild(ProjectItem *child)
{
    _childrens.removeOne(child);
    _childrensMap.remove(child->fileName());
    delete child;
}

void ProjectItem::remove()
{
    if (!_parentItem)
    {
        return;
    }
    _parentItem->removeChild(this);
}

void ProjectItem::clear()
{
    qDeleteAll(_childrens);
    _childrens.clear();
    _childrensMap.clear();
}

void ProjectItem::addFileItem(const QString &path)
{
    addChild(new ProjectItem(_info.project(), path, ProjectItem::IndividualFile, _model));
}

void ProjectItem::addRealDirItem(const QString &path)
{
    addChild(new ProjectItem(_info.project(), path, ProjectItem::RealDir, _model));
}

void ProjectItem::addLogicDirItem(const QString &name)
{
    addChild(new ProjectItem(_info.project(), name, ProjectItem::LogicDir, _model));
}

QVariant ProjectItem::data(int column, int role) const
{
    Q_UNUSED(column)
    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
        return _info.fileName();
    }
    if (role == ProjectItemModel::FilePathRole)
    {
        return _info.filePath();
    }
    if (role == ProjectItemModel::FileNameRole)
    {
        return _info.fileName();
    }
    if (role == Qt::FontRole || role == Qt::TextColorRole || role == Qt::ToolTipRole)
    {
        if (_type == LogicDir)
        {
            return QVariant();
        }

        // font role
        if (role == Qt::FontRole)
        {
            if (_info.isModified() || _info.isValidated())
            {
                QFont font;
                font.setBold(true);
                if (_info.isOpened())
                {
                    font.setUnderline(true);
                }
                return font;
            }
            if (!_info.isTracked())
            {
                QFont font;
                font.setItalic(true);
                if (_info.isOpened())
                {
                    font.setUnderline(true);
                }
                return font;
            }
            if (_info.isOpened())
            {
                QFont font;
                font.setUnderline(true);
                return font;
            }
            return QVariant();
        }

        // text color
        if (role == Qt::TextColorRole)
        {
            if (_info.isValidated())
            {
                if (_info.isModified())
                {
                    return QVariant(QColor(255, 127, 0));
                }
                else
                {
                    return QVariant(QColor(0, 255, 0));
                }
            }
            if (!_info.isTracked())
            {
                return QColor(127, 127, 127);
            }
            return QVariant();
        }

        // tooltip
        if (role == Qt::ToolTipRole)
        {
            return _info.toolTips();
        }
    }
    return QVariant();
}

ProjectItem *ProjectItem::parentItem() const
{
    return _parentItem;
}

const FileProjectInfo &ProjectItem::info() const
{
    return _info;
}

/*bool ProjectItem::haveChildren() const
{
    if (_type == RealDir && _loaded == false)
        return true;
    return !_childrens.empty();
}

bool ProjectItem::canFetchMore() const
{
    if (_type == RealDir && _loaded == false)
        return true;
    return false;
}

void ProjectItem::fetchMore()
{
    updateModif(fileName());
    _loaded = true;
}*/

void ProjectItem::updateModif(const QString &path)
{
    Q_UNUSED(path)
    switch (_type)
    {
    case ProjectItem::RealDir:
    {
        _model->prepareModif();
        QSet<QString> files;
        QDirIterator it(filePath(), QDir::NoDotAndDotDot | QDir::AllEntries | QDir::Hidden);
        while (it.hasNext())
        {
            QString mfilePath = it.next();
            QFileInfo info(mfilePath);
            files.insert(info.fileName());
            if (!_childrensMap.contains(info.fileName()))
            {
                if (info.isDir())
                {
                    addChild(new ProjectItem(_info.project(), info.filePath(), RealDir, _model));
                }
                else
                {
                    addChild(new ProjectItem(_info.project(), info.filePath(), DirFile, _model));
                }
            }
        }

        QSet<QString> oldFiles = _childrensMap.keys().toSet();
        oldFiles.subtract(files);
        foreach (QString removedFile, oldFiles)
        {
            QHash<QString, ProjectItem *>::const_iterator i = _childrensMap.find(removedFile);
            if (i != _childrensMap.end())
            {
                removeChild(*i);
            }
        }

        _model->endModif();
        break;
    }
    case ProjectItem::DirFile:
        // no watch
        break;
    case ProjectItem::LogicDir:
        // no watch
        break;
    case ProjectItem::IndividualFile:
        // TODO
        break;
    }
}

ProjectItem::Type ProjectItem::type() const
{
    return _type;
}
