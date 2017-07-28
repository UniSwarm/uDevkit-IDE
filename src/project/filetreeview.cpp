#include "filetreeview.h"

#include <QHeaderView>
#include <QMouseEvent>

FileTreeView::FileTreeView(Project *project, QWidget *parent)
    : QTreeView(parent), _project(project)
{
    _proxy = new FileProjectProxyModel(project);
    _proxy->setSourceModel(_project->fileItemModel());
    _proxy->setHiddenFilter(QRegExp("(nbproject|bin|.*build.*|.*\\.git$|rtsim)", Qt::CaseInsensitive, QRegExp::RegExp));

    setModel(_proxy);
    setEditTriggers(QAbstractItemView::EditKeyPressed);
    setRootIndex(_proxy->mapFromSource(_project->fileItemModel()->index(project->rootPath())));
    for (int i = 1; i < _proxy->columnCount(); ++i)
        hideColumn(i);
    setStyleSheet("QTreeView { selection-background-color: transparent; }");
    header()->close();
}

FileProjectProxyModel *FileTreeView::proxy() const
{
    return _proxy;
}

void FileTreeView::mouseDoubleClickEvent(QMouseEvent *event)
{
    QTreeView::mouseDoubleClickEvent(event);

    if (!event->buttons().testFlag(Qt::LeftButton))
        return;

    const QPersistentModelIndex persistent = indexAt(event->pos());

    const QModelIndex &indexFile = _proxy->mapToSource(persistent);
    if (_project->fileItemModel()->isDir(indexFile))
        emit doubleClickedDir(_project->fileItemModel()->filePath(indexFile));
    else
        emit doubleClickedFile(_project->fileItemModel()->filePath(indexFile));
}
