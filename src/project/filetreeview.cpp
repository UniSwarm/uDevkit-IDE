#include "filetreeview.h"

#include <QDebug>
#include <QHeaderView>
#include <QItemSelectionModel>
#include <QInputDialog>
#include <QMenu>
#include <QMessageBox>
#include <QMouseEvent>

#include "fileprojectinfo.h"
#include "mainwindow.h"

FileTreeView::FileTreeView(Project *project, QWidget *parent)
    : QTreeView(parent), _project(project)
{
    _proxy = new FileProjectProxyModel(project);
    _proxy->setSourceModel(_project->projectItemModel());
    _proxy->setHiddenFilter(QRegExp("^$"));

    setModel(_proxy);
    setEditTriggers(QAbstractItemView::EditKeyPressed);
    //setRootIndex(_proxy->mapFromSource(_project->projectItemModel()->index(project->rootPath())));
    for (int i = 1; i < _proxy->columnCount(); ++i)
        hideColumn(i);
    setStyleSheet("QTreeView { selection-background-color: transparent; }");
    header()->close();
}

FileProjectProxyModel *FileTreeView::proxy() const
{
    return _proxy;
}

void FileTreeView::selectFile(const QString &fileName)
{
    QModelIndex fileIndex = _project->projectItemModel()->index(fileName);
    if (!fileIndex.isValid())
        return;

    QModelIndex proxyIndex = _proxy->mapFromSource(fileIndex);
    if (!proxyIndex.isValid())
        return;

    selectionModel()->setCurrentIndex(proxyIndex, QItemSelectionModel::ClearAndSelect);
}

void FileTreeView::setHiddenFilter(const QRegExp &regExp)
{
    if (!regExp.isValid() || regExp.isEmpty())
        _proxy->setHiddenFilter(QRegExp("^$"));
    else
        _proxy->setHiddenFilter(regExp);
}

void FileTreeView::setHiddenFilter(const QString &pattern)
{
    setHiddenFilter(QRegExp(pattern, Qt::CaseInsensitive, QRegExp::Wildcard));
}

void FileTreeView::setShowFilter(const QRegExp &regExp)
{
    _proxy->setShowFilter(regExp);
}

void FileTreeView::setShowFilter(const QString &pattern)
{
    setShowFilter(QRegExp(pattern, Qt::CaseInsensitive, QRegExp::Wildcard));
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

    // file commands
    QAction *fileCreateAction = nullptr;
    if(_project->projectItemModel()->isDir(indexFile))
    {
        fileCreateAction = menu.addAction("Add new file here");
    }
    QAction *fileRenameAction = menu.addAction("Rename");
    fileRenameAction->setShortcut(QKeySequence(Qt::Key_F2));
    QAction *fileRemoveAction = nullptr, *dirRemoveAction = nullptr, *openDirAction = nullptr;
    if(_project->projectItemModel()->isDir(indexFile))
    {
        dirRemoveAction = menu.addAction("Remove directory");
        dirRemoveAction->setShortcut(QKeySequence::Delete);

        openDirAction = menu.addAction("Open directory as project");
    }
    else
    {
        fileRemoveAction = menu.addAction("Remove file");
        fileRemoveAction->setShortcut(QKeySequence::Delete);
    }

    // git commands
    QAction *versionValidAction = nullptr, *versionInvalidAction = nullptr, *versionCheckoutAction = nullptr;
    FileProjectInfo info(_project->projectItemModel()->filePath(indexFile), _project);
    if (_project->versionControl()->isValid())
    {
        menu.addSeparator();
        if (!info.isTracked() || info.isModified())
            versionValidAction = menu.addAction(_project->versionControl()->versionControlName()+" add");
        if (info.isValidated())
            versionInvalidAction = menu.addAction(_project->versionControl()->versionControlName()+" reset HEAD");
        if (info.isModified())
            versionCheckoutAction = menu.addAction(_project->versionControl()->versionControlName()+" checkout");
    }

    // exec menu
    QAction *trigered = menu.exec(event->globalPos());
    if(trigered == nullptr)
        return;

    // analyse clicked menu
    if (trigered == fileCreateAction)
    {
        QString fileName = QInputDialog::getText(this, "New file name", "Enter a name for this new file");
        QString filePath = _project->projectItemModel()->filePath(indexFile) + "/" + fileName;
        QFile file(filePath);
        if (!file.exists())
        {
            if(!file.open(QIODevice::WriteOnly))
                return;
            file.close();
            emit openedFile(filePath);
        }
    }
    else if (trigered == dirRemoveAction)
    {
        if(QMessageBox::question(this, "Remove directory?", QString("Do you realy want to remove '%1'?")
                                 .arg(_project->projectItemModel()->fileName(indexFile))) == QMessageBox::Yes)
            _project->projectItemModel()->rmdir(indexFile);
    }
    else if (trigered == fileRemoveAction)
    {
        if (QMessageBox::question(this, "Remove file?", QString("Do you realy want to remove '%1'?")
                                  .arg(_project->projectItemModel()->fileName(indexFile))) == QMessageBox::Yes)
            _project->projectItemModel()->remove(indexFile);
    }
    else if (trigered == fileRenameAction)
        edit(index);
    else if (trigered == openDirAction)
    {
        Project *project = new Project(_project->projectItemModel()->filePath(indexFile));
        MainWindow *w = new MainWindow(project);
        w->show();
    }
    else if (trigered == versionValidAction)
        _project->versionControl()->validFile(QSet<QString>()<<info.filePath());
    else if (trigered == versionInvalidAction)
        _project->versionControl()->inValidFile(QSet<QString>()<<info.filePath());
    else if (trigered == versionCheckoutAction)
        if (QMessageBox::question(this, "Checkout file?", QString("Do you realy want to checkout '%1'?\nIt will be restored to the last valid state.")
                                  .arg(_project->projectItemModel()->fileName(indexFile))) == QMessageBox::Yes)
            _project->versionControl()->checkoutFile(QSet<QString>()<<info.filePath());
}

void FileTreeView::mouseReleaseEvent(QMouseEvent *event)
{
    QTreeView::mouseReleaseEvent(event);

    if (event->button() == Qt::MidButton)
    {
        const QPersistentModelIndex index = indexAt(event->pos());
        if (!index.isValid())
            return;

        const QModelIndex &indexFile = _proxy->mapToSource(index);
        if (!_project->projectItemModel()->isDir(indexFile))
            emit closedFile(_project->projectItemModel()->filePath(indexFile));
    }
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
    if (!_project->projectItemModel()->isDir(indexFile))
        emit openedFile(_project->projectItemModel()->filePath(indexFile));
}

void FileTreeView::keyPressEvent(QKeyEvent *event)
{
    QTreeView::keyPressEvent(event);

    if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
    {
        const QPersistentModelIndex index = selectionModel()->currentIndex();
        if (!index.isValid())
            return;

        const QModelIndex &indexFile = _proxy->mapToSource(index);
        if (!_project->projectItemModel()->isDir(indexFile))
            emit openedFile(_project->projectItemModel()->filePath(indexFile));
    }
}
