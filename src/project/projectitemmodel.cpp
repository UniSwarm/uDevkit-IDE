#include "projectitemmodel.h"

#include <QDebug>

#include "fileprojectitemmodel.h"

#include "project.h"

ProjectItemModel::ProjectItemModel(Project *project) :
    _project(project)
{
    _externalFiles = nullptr;
    _root = new ProjectItem(_project, "");
    _iconProvider = new QFileIconProvider;
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
        return itemPtr->info().isDir() || (itemPtr->type() == ProjectItem::LogicDir);
    return false;
}

QString ProjectItemModel::filePath(const QModelIndex &index) const
{
    const ProjectItem *itemPtr = item(index);
    if (itemPtr)
        return itemPtr->info().filePath();
    return QString();
}

QString ProjectItemModel::fileName(const QModelIndex &index) const
{
    const ProjectItem *itemPtr = item(index);
    if (itemPtr)
        return itemPtr->info().fileName();
    return QString();
}

bool ProjectItemModel::rmdir(const QModelIndex &index)
{
    QString mfileName = filePath(index);
    if (mfileName.isEmpty())
        return false;
    return QDir(mfileName).removeRecursively();
}

bool ProjectItemModel::remove(const QModelIndex &index)
{
    QString mfileName = filePath(index);
    if (mfileName.isEmpty())
        return false;
    return QFile(mfileName).remove();
}

void ProjectItemModel::addExternalSource(QSet<QString> sourceFiles)
{
    emit layoutAboutToBeChanged();
    if (!_externalFiles)
    {
        _externalFiles = new ProjectItem(_project, "ext src", ProjectItem::LogicDir);
        _root->addChild(_externalFiles);
    }
    foreach (QString filePath, sourceFiles)
    {
        if (filePath.startsWith(_project->rootPath()))
            continue;

        ProjectItem *item = new ProjectItem(_project, filePath);
        _externalFiles->addChild(item);
        _pathCache.insert(filePath, item);
    }
    emit layoutChanged();
}

void ProjectItemModel::removeExternalSource(QSet<QString> sourceFiles)
{
    if (!_externalFiles)
        return;
    emit layoutAboutToBeChanged();
    foreach (QString filePath, sourceFiles)
    {
        if (filePath.startsWith(_project->rootPath()))
            continue; // not external source
        ProjectItem *item = _pathCache[filePath];
        if (!item)
            continue;
        _externalFiles->removeChild(item);
        _pathCache.remove(filePath);
    }
    emit layoutChanged();
}

void ProjectItemModel::filesUpdated(QSet<QString> filesPath)
{
    foreach (QString path, filesPath)
    {
        QModelIndex id = index(path);
        if (id.isValid())
            emit dataChanged(id, id);
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

const ProjectItem *ProjectItemModel::item(const QModelIndex &index) const
{
    if (!index.isValid())
        return nullptr;
    return static_cast<ProjectItem*>(index.internalPointer());
}

QModelIndex ProjectItemModel::index(const QString path) const
{
    QModelIndexList list = match(index(0,0), ProjectItemModel::FilePathRole, path, -1, Qt::MatchRecursive);
    if (list.isEmpty())
        return QModelIndex();
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
        return QModelIndex();

    ProjectItem *parentItem;

    if (!parent.isValid())
        parentItem = _root;
    else
        parentItem = static_cast<ProjectItem*>(parent.internalPointer());

    ProjectItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex ProjectItemModel::parent(const QModelIndex &child) const
{
    if (!child.isValid() || child.internalPointer()==NULL)
        return QModelIndex();

    ProjectItem *childItem = static_cast<ProjectItem*>(child.internalPointer());
    ProjectItem *parentItem = childItem->parentItem();

    if (parentItem == _root)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int ProjectItemModel::rowCount(const QModelIndex &parent) const
{
    ProjectItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = _root;
    else
        parentItem = static_cast<ProjectItem*>(parent.internalPointer());

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
        return QVariant();
    ProjectItem *item = static_cast<ProjectItem*>(index.internalPointer());

    if (role == Qt::DecorationRole)
    {
        if (item->type() == ProjectItem::LogicDir)
            return _iconProvider->icon(QFileIconProvider::Folder);
        return _iconProvider->icon(item->info());
    }

    return item->data(index.column(), role);
}

Qt::ItemFlags ProjectItemModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return QAbstractItemModel::flags(index);
}
