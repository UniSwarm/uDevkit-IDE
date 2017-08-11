#include "projectitem.h"

#include "project.h"

#include <QDebug>
#include <QFont>

ProjectItem::ProjectItem(Project *project, const QString path, Type type)
    : QObject(nullptr), _type(type), _info(path, project)
{
    _parentItem = nullptr;

    _watcher = new QFileSystemWatcher();
    if (type == RealDir)
    {
        QFileInfoList list = QDir(path).entryInfoList(QStringList(), QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
        foreach (QFileInfo info, list)
        {
            if (info.isDir())
                addChild(new ProjectItem(project, info.filePath(), RealDir));
            else
                addChild(new ProjectItem(project, info.filePath(), File));
        }

        _watcher->addPath(QDir::cleanPath(path)+"/");
        connect(_watcher, &QFileSystemWatcher::directoryChanged, this, &ProjectItem::updateModif);
        connect(_watcher, &QFileSystemWatcher::fileChanged, this, &ProjectItem::updateModif);
    }

    if (type == File)
    {
        _watcher->addPath(QDir::cleanPath(path));
        connect(_watcher, &QFileSystemWatcher::fileChanged, this, &ProjectItem::updateModif);
    }
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

void ProjectItem::removeChild(ProjectItem *child)
{
    _childrens.removeOne(child);
    delete child;
}

void ProjectItem::addFileItem(const QString &path)
{
    addChild(new ProjectItem(_info.project(), path));
}

void ProjectItem::addRealDirItem(const QString &path)
{
    addChild(new ProjectItem(_info.project(), path, ProjectItem::RealDir));
}

void ProjectItem::addLogicDirItem(const QString &name)
{
    addChild(new ProjectItem(_info.project(), name, ProjectItem::LogicDir));
}

QVariant ProjectItem::data(int column, int role) const
{
    Q_UNUSED(column)
    if (role == Qt::DisplayRole || role == Qt::EditRole)
        return _info.fileName();
    if (role == ProjectItemModel::FilePathRole)
        return _info.filePath();
    if (role == ProjectItemModel::FileNameRole)
        return _info.fileName();
    if (role == Qt::FontRole || role == Qt::TextColorRole || role == Qt::ToolTipRole)
    {
        if (_type == LogicDir)
            return QVariant();

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

void ProjectItem::updateModif(const QString &path)
{
    qDebug()<<_info.fileName()<<path;
}

ProjectItem::Type ProjectItem::type() const
{
    return _type;
}
