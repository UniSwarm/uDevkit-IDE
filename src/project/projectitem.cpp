#include "projectitem.h"

#include "project.h"

#include <QFont>

ProjectItem::ProjectItem(Project *project, const QString path)
    : QObject(nullptr), _info(path, project)
{
    _parentItem = nullptr;
}

ProjectItem::~ProjectItem()
{
    qDeleteAll(_childrens);
}

int ProjectItem::count() const
{
    return _childrens.count();
}

ProjectItem *ProjectItem::child(int row) const
{
    return _childrens.at(row);
}

int ProjectItem::row() const
{
    if (_parentItem)
        return _parentItem->_childrens.indexOf(const_cast<ProjectItem*>(this));

    return 0;
}

void ProjectItem::addChild(ProjectItem *child)
{
    child->_parentItem = this;
    _childrens.append(child);
}

QVariant ProjectItem::data(int column, int role) const
{
    if (role == Qt::DisplayRole)
        return _info.fileName();
    if (role == Qt::FontRole || role == Qt::TextColorRole || role == Qt::ToolTipRole)
    {
        // font role
        if (role == Qt::FontRole)
        {
            if (_info.isModified() || _info.isValidated())
            {
                QFont font;
                font.setBold(true);
                if (_info.isOpened())
                    font.setUnderline(true);
                return font;
            }
            if (!_info.isTracked())
            {
                QFont font;
                font.setItalic(true);
                if (_info.isOpened())
                    font.setUnderline(true);
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
                return QVariant(QColor(0, 255, 0));
            if (!_info.isTracked())
                return QColor(127, 127, 127);
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
