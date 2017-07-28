#include "filetreeview.h"

#include <QHeaderView>
#include <QMenu>
#include <QMessageBox>
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

    const QPersistentModelIndex index = indexAt(event->pos());
    if (!index.isValid())
        return;

    const QModelIndex &indexFile = _proxy->mapToSource(index);
    if (_project->fileItemModel()->isDir(indexFile))
        emit doubleClickedDir(_project->fileItemModel()->filePath(indexFile));
    else
        emit doubleClickedFile(_project->fileItemModel()->filePath(indexFile));
}

void FileTreeView::contextMenuEvent(QContextMenuEvent *event)
{
    const QModelIndex &index = indexAt(event->pos());
    if (!index.isValid())
        return;
    const QModelIndex &indexFile = _proxy->mapToSource(index);
    if (!indexFile.isValid())
        return;

    QMenu menu;

    // file
    QAction *fileRenameAction = menu.addAction("Rename");
    fileRenameAction->setShortcut(QKeySequence(Qt::Key_F2));
    QAction *fileRemoveAction = nullptr, *dirRemoveAction = nullptr;
    if(_project->fileItemModel()->isDir(indexFile))
    {
        dirRemoveAction = menu.addAction("Remove directory");
        dirRemoveAction->setShortcut(QKeySequence::Delete);
    }
    else
    {
        fileRemoveAction = menu.addAction("Remove file");
        fileRemoveAction->setShortcut(QKeySequence::Delete);
    }

    QAction *trigered = menu.exec(event->globalPos());
    if(trigered == nullptr)
        return;
    if(trigered == dirRemoveAction)
    {
        if(QMessageBox::question(this, "Remove file?", QString("Do you realy want to remove '%1'?").arg(_project->fileItemModel()->fileName(indexFile))) == QMessageBox::Yes)
            _project->fileItemModel()->rmdir(indexFile);
    }
    else if(trigered == fileRemoveAction)
    {
        if(QMessageBox::question(this, "Remove file?", QString("Do you realy want to remove '%1'?").arg(_project->fileItemModel()->fileName(indexFile))) == QMessageBox::Yes)
            _project->fileItemModel()->remove(indexFile);
    }
    else if(trigered == fileRenameAction)
        edit(index);
}
