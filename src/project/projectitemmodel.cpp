#include "projectitemmodel.h"

#include <QDebug>

#include "fileprojectitemmodel.h"

ProjectItemModel::ProjectItemModel(Project *project) :
    _project(project)
{
    _root = new ProjectItem(_project, "");
    _iconProvider = new QFileIconProvider;
}

ProjectItemModel::~ProjectItemModel()
{
    delete _root;
}

void ProjectItemModel::addFileItem(const QString &path)
{
    emit layoutAboutToBeChanged();
    _root->addChild(new ProjectItem(_project, path));
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
