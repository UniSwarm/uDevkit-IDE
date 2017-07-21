#include "fileprojectitemmodel.h"

#include "project.h"

#include <QFont>
#include <QDebug>

FileProjectItemModel::FileProjectItemModel(Project *project)
{
    _project = project;
}

void FileProjectItemModel::filesUpdated(QSet<QString> filesPath)
{
    foreach (QString path, filesPath)
    {
        QModelIndex id = index(path);
        if (id.isValid())
            emit dataChanged(id, id);
    }
}

QVariant FileProjectItemModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::FontRole)
    {
        if (!_project || !index.isValid())
            return QFileSystemModel::data(index, role);
        if (_project->versionControl()->isFileModified(filePath(index)))
        {
            QFont font;
            font.setBold(true);
            return font;
        }
        return QVariant();
    }
    return QFileSystemModel::data(index, role);
}
