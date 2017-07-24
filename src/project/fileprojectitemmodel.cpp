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
        bool modified = false, tracked = true;
        if (!_project || !index.isValid())
            return QFileSystemModel::data(index, role);
        if (isDir(index))
            modified = _project->versionControl()->isDirModified(filePath(index));
        else
        {
            modified = _project->versionControl()->isFileModified(filePath(index));
            tracked = _project->versionControl()->isFileTracked(filePath(index));
        }
        if (modified)
        {
            QFont font;
            font.setBold(true);
            return font;
        }
        if (!tracked)
        {
            QFont font;
            font.setItalic(true);
            return font;
        }
        return QVariant();
    }
    return QFileSystemModel::data(index, role);
}
