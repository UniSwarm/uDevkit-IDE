#include "fileprojectitemmodel.h"

#include "fileprojectinfo.h"
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
    if (role == Qt::FontRole || role == Qt::TextColorRole || role == Qt::ToolTipRole)
    {
        FileProjectInfo info(filePath(index), _project);

        // font role
        if (role == Qt::FontRole)
        {
            if (info.isModified() || info.isValidated())
            {
                QFont font;
                font.setBold(true);
                if (info.isOpened())
                    font.setUnderline(true);
                return font;
            }
            if (!info.isTracked())
            {
                QFont font;
                font.setItalic(true);
                if (info.isOpened())
                    font.setUnderline(true);
                return font;
            }
            if (info.isOpened())
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
            if (info.isValidated())
                return QVariant(QColor(0, 255, 0));
            if (!info.isTracked())
                return QColor(127, 127, 127);
            return QVariant();
        }

        // tooltip
        if (role == Qt::ToolTipRole)
        {
            return info.toolTips();
        }
    }
    return QFileSystemModel::data(index, role);
}
