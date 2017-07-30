#include "fileprojectitemmodel.h"

#include "project.h"

#include <QFont>
#include <QDebug>

FileProjectItemModel::FileProjectItemModel(Project *project)
{
    _project = project;
    setReadOnly(false);
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
    if (role == Qt::FontRole || role == Qt::TextColorRole)
    {
        bool modified = false, tracked = true, validated = false;
        if (!_project || !index.isValid())
            return QFileSystemModel::data(index, role);
        if (isDir(index))
        {
            modified = _project->versionControl()->isDirModified(filePath(index));
            tracked = _project->versionControl()->isDirTracked(filePath(index));
        }
        else
        {
            modified = _project->versionControl()->isFileModified(filePath(index));
            tracked = _project->versionControl()->isFileTracked(filePath(index));
            validated = _project->versionControl()->isFileValidated(filePath(index));
        }

        // font role
        if (role == Qt::FontRole)
        {
            if (modified || validated)
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

        // text color
        if (role == Qt::TextColorRole)
        {
            if (validated)
                return QVariant(QColor(0, 255, 0));
            if (!tracked)
                return QColor(127, 127, 127);
            return QVariant();
        }
    }
    return QFileSystemModel::data(index, role);
}
