#include "projectitemmodel.h"

#include <QDebug>

#include "projectitemmodel.h"

#include "project.h"
#include "projecticonprovider.h"

ProjectItemModel::ProjectItemModel(Project *project)
    : _project(project)
{
    _externalFiles = Q_NULLPTR;
    _otherFiles = Q_NULLPTR;
    _root = new ProjectItem(_project, "", ProjectItem::LogicDir, this);
    _iconProvider = new ProjectIconProvider();
}

ProjectItemModel::~ProjectItemModel()
{
    delete _root;
    delete _iconProvider;
}

void ProjectItemModel::addRealDirItem(const QString &path)
{
    emit layoutAboutToBeChanged();
    _root->addRealDirItem(path);
    emit layoutChanged();
}

bool ProjectItemModel::isDir(const QModelIndex &index) const
{
    const ProjectItem *itemPtr = item(index);
    if (itemPtr)
    {
        return itemPtr->info().isDir() || (itemPtr->type() == ProjectItem::LogicDir);
    }
    return false;
}

bool ProjectItemModel::isFile(const QModelIndex &index) const
{
    const ProjectItem *itemPtr = item(index);
    if (itemPtr)
    {
        return itemPtr->info().isFile() || (itemPtr->type() == ProjectItem::LogicDir);
    }
    return false;
}

QString ProjectItemModel::filePath(const QModelIndex &index) const
{
    const ProjectItem *itemPtr = item(index);
    if (itemPtr)
    {
        return itemPtr->info().filePath();
    }
    return QString();
}

QString ProjectItemModel::fileName(const QModelIndex &index) const
{
    const ProjectItem *itemPtr = item(index);
    if (itemPtr)
    {
        return itemPtr->info().fileName();
    }
    return QString();
}

bool ProjectItemModel::rmdir(const QModelIndex &index)
{
    QString mfileName = filePath(index);
    if (mfileName.isEmpty())
    {
        return false;
    }
    ProjectItem *mitem = static_cast<ProjectItem *>(index.internalPointer());
    if (mitem == Q_NULLPTR)
    {
        return false;
    }
    beginRemoveRows(index.parent(), mitem->row(), mitem->row());
    bool valid = QDir(mfileName).removeRecursively();
    if (valid)
    {
        mitem->remove();
    }
    endRemoveRows();
    return valid;
}

bool ProjectItemModel::remove(const QModelIndex &index)
{
    QString mfileName = filePath(index);
    if (mfileName.isEmpty())
    {
        return false;
    }
    ProjectItem *mitem = static_cast<ProjectItem *>(index.internalPointer());
    if (mitem == Q_NULLPTR)
    {
        return false;
    }
    beginRemoveRows(index.parent(), mitem->row(), mitem->row());
    bool valid = QFile(mfileName).remove();
    if (valid)
    {
        mitem->remove();
    }
    endRemoveRows();
    return valid;
}

void ProjectItemModel::addExternalSource(QSet<QString> sourceFiles)
{
    emit layoutAboutToBeChanged();
    foreach (QString filePath, sourceFiles)
    {
        if (filePath.startsWith(_project->rootPath()))
        {
            continue; // not an external source
        }

        // qDebug() << "addExternalSource" << filePath;

        if (!_externalFiles)
        {
            _externalFiles = new ProjectItem(_project, tr("ext src"), ProjectItem::LogicDir, this);
            _root->addChild(_externalFiles);
        }

        QFileInfo info(filePath);
        QDir dir = info.absoluteDir();

        ProjectItem *parent = _externalFiles->child(dir.dirName());
        if (!parent)
        {
            parent = new ProjectItem(_project, dir.absolutePath(), ProjectItem::LogicDir, this);
            _externalFiles->addChild(parent);
        }

        ProjectItem *item = new ProjectItem(_project, filePath, ProjectItem::IndividualFile, this);
        parent->addChild(item);
        _pathCache.insert(filePath, item);
    }
    emit layoutChanged();
}

void ProjectItemModel::removeExternalSource(QSet<QString> sourceFiles)
{
    if (!_externalFiles)
    {
        return;
    }
    emit layoutAboutToBeChanged();
    foreach (QString filePath, sourceFiles)
    {
        if (filePath.startsWith(_project->rootPath()))
        {
            continue; // not external source
        }

        QFileInfo info(filePath);
        QDir dir = info.absoluteDir();

        ProjectItem *parent = _externalFiles->child(dir.dirName());
        if (!parent)
        {
            continue;
        }

        ProjectItem *item = _pathCache[filePath];
        if (!item)
        {
            continue;
        }
        parent->removeChild(item);
        _pathCache.remove(filePath);

        if (parent->count() == 0)
        {
            _externalFiles->removeChild(parent);
        }
    }
    emit layoutChanged();
}

void ProjectItemModel::addOtherSource(QSet<QString> sourceFiles)
{
    emit layoutAboutToBeChanged();
    foreach (QString filePath, sourceFiles)
    {
        if (filePath.startsWith(_project->rootPath()))
        {
            continue; // not an external source
        }
        if (_pathCache.contains(filePath))
        {
            return;
        }

        if (!_otherFiles)
        {
            _otherFiles = new ProjectItem(_project, "opened files", ProjectItem::LogicDir, this);
            _root->addChild(_otherFiles);
        }

        ProjectItem *item = new ProjectItem(_project, filePath, ProjectItem::IndividualFile, this);
        _otherFiles->addChild(item);
        _pathCache.insert(filePath, item);
    }
    emit layoutChanged();
}

void ProjectItemModel::removeOtherSource(QSet<QString> sourceFiles)
{
    if (!_otherFiles)
    {
        return;
    }
    emit layoutAboutToBeChanged();
    foreach (QString filePath, sourceFiles)
    {
        ProjectItem *item = _pathCache[filePath];
        if (!item)
        {
            continue;
        }
        _otherFiles->removeChild(item);
        _pathCache.remove(filePath);
    }
    emit layoutChanged();
}

void ProjectItemModel::prepareModif()
{
    emit layoutAboutToBeChanged();
}

void ProjectItemModel::endModif()
{
    emit layoutChanged();
}

void ProjectItemModel::filesUpdated(QSet<QString> filesPath)
{
    foreach (QString path, filesPath)
    {
        QModelIndex id = index(path);
        if (id.isValid())
        {
            emit dataChanged(id, id);
        }
    }
}

void ProjectItemModel::addLogicDirItem(const QString &name)
{
    emit layoutAboutToBeChanged();
    _root->addLogicDirItem(name);
    emit layoutChanged();
}

void ProjectItemModel::addItem(ProjectItem *item)
{
    emit layoutAboutToBeChanged();
    _root->addChild(item);
    emit layoutChanged();
}

void ProjectItemModel::clear()
{
    emit layoutAboutToBeChanged();
    _root->clear();
    _externalFiles = Q_NULLPTR;
    emit layoutChanged();
}

const ProjectItem *ProjectItemModel::item(const QModelIndex &index) const
{
    if (!index.isValid())
    {
        return Q_NULLPTR;
    }
    return static_cast<ProjectItem *>(index.internalPointer());
}

QModelIndex ProjectItemModel::index(const QString path) const
{
    QModelIndexList list = match(index(0, 0), ProjectItemModel::FilePathRole, path, -1, Qt::MatchRecursive);
    if (list.isEmpty())
    {
        return QModelIndex();
    }
    return list.first();
}

/*QModelIndex ProjectItemModel::index(const ProjectItem *item) const
{

}*/

void ProjectItemModel::addFileItem(const QString &path)
{
    emit layoutAboutToBeChanged();
    _root->addFileItem(path);
    emit layoutChanged();
}

QModelIndex ProjectItemModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
    {
        return QModelIndex();
    }

    ProjectItem *parentItem;

    if (!parent.isValid())
    {
        parentItem = _root;
    }
    else
    {
        parentItem = static_cast<ProjectItem *>(parent.internalPointer());
    }

    ProjectItem *childItem = parentItem->child(row);
    if (childItem)
    {
        return createIndex(row, column, childItem);
    }
    else
    {
        return QModelIndex();
    }
}

QModelIndex ProjectItemModel::parent(const QModelIndex &child) const
{
    if (!child.isValid() || child.internalPointer() == Q_NULLPTR)
    {
        return QModelIndex();
    }

    ProjectItem *childItem = static_cast<ProjectItem *>(child.internalPointer());
    ProjectItem *parentItem = childItem->parentItem();

    if (parentItem == _root)
    {
        return QModelIndex();
    }

    return createIndex(parentItem->row(), 0, parentItem);
}

int ProjectItemModel::rowCount(const QModelIndex &parent) const
{
    ProjectItem *parentItem;
    if (parent.column() > 0)
    {
        return 0;
    }

    if (!parent.isValid())
    {
        parentItem = _root;
    }
    else
    {
        parentItem = static_cast<ProjectItem *>(parent.internalPointer());
    }

    return parentItem->count();
}

int ProjectItemModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 1;
}

QVariant ProjectItemModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }
    ProjectItem *item = static_cast<ProjectItem *>(index.internalPointer());

    if (role == Qt::DecorationRole)
    {
        if (item->type() == ProjectItem::LogicDir)
        {
            return _iconProvider->icon(QFileIconProvider::Folder);
        }
        return _iconProvider->icon(item->info());
    }

    return item->data(index.column(), role);
}

Qt::ItemFlags ProjectItemModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
    {
        return nullptr;
    }
    Qt::ItemFlags baseFlags = Qt::ItemIsSelectable | Qt::ItemIsEnabled;

    ProjectItem *item = static_cast<ProjectItem *>(index.internalPointer());
    if (item->type() != ProjectItem::LogicDir)
    {
        return baseFlags | Qt::ItemIsEditable;
    }

    return baseFlags;
}

bool ProjectItemModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    Q_UNUSED(role)
    ProjectItem *item = static_cast<ProjectItem *>(index.internalPointer());
    return item->info().dir().rename(item->info().fileName(), value.toString());
}

/*bool ProjectItemModel::hasChildren(const QModelIndex &parent) const
{
    return false;//item(parent)->haveChildren();
}

void ProjectItemModel::fetchMore(const QModelIndex &parent)
{
    emit layoutAboutToBeChanged();
    static_cast<ProjectItem*>(parent.internalPointer())->fetchMore();
    emit layoutChanged();
}

bool ProjectItemModel::canFetchMore(const QModelIndex &parent) const
{
    return false;//item(parent)->canFetchMore();
}*/
