#include "projecttreeview.h"

#include <QDebug>
#include <QHeaderView>
#include <QItemSelectionModel>
#include <QInputDialog>
#include <QMenu>
#include <QMessageBox>
#include <QMouseEvent>
#include <QProcess>
#include <QApplication>

#include "fileprojectinfo.h"
#include "mainwindow.h"

ProjectTreeView::ProjectTreeView(Project *project, QWidget *parent)
    : QTreeView(parent), _project(project)
{
    _proxy = new ProjectItemProxyModel(project);
    _proxy->setSourceModel(_project->projectItemModel());
    setHiddenFilter(QRegExp(""));
    _proxy->sort(0, Qt::AscendingOrder);

    setSelectionMode(QAbstractItemView::ExtendedSelection);

    setModel(_proxy);
    setEditTriggers(QAbstractItemView::EditKeyPressed);
    for (int i = 1; i < _proxy->columnCount(); ++i)
        hideColumn(i);
    setStyleSheet("QTreeView { selection-background-color: transparent; }");
    header()->close();

    _removeAction = new QAction(this);
    _removeAction->setText("remove");
    _removeAction->setShortcut(QKeySequence::Delete);
    _removeAction->setShortcutContext(Qt::WidgetShortcut);
    connect(_removeAction, SIGNAL(triggered(bool)), this, SLOT(remove()));
    addAction(_removeAction);

    expandToDepth(0);
}

ProjectItemProxyModel *ProjectTreeView::proxy() const
{
    return _proxy;
}

void ProjectTreeView::selectFile(const QString &fileName)
{
    QModelIndex fileIndex = _project->projectItemModel()->index(fileName);
    if (!fileIndex.isValid())
        return;

    QModelIndex proxyIndex = _proxy->mapFromSource(fileIndex);
    if (!proxyIndex.isValid())
        return;

    selectionModel()->setCurrentIndex(proxyIndex, QItemSelectionModel::ClearAndSelect);
}

void ProjectTreeView::setHiddenFilter(const QRegExp &regExp)
{
    if (!regExp.isValid() || regExp.isEmpty())
        _proxy->setHiddenFilter(QRegExp("^$"));
    else
        _proxy->setHiddenFilter(regExp);
}

void ProjectTreeView::setHiddenFilter(const QString &pattern)
{
    setHiddenFilter(QRegExp(pattern, Qt::CaseInsensitive, QRegExp::Wildcard));
}

void ProjectTreeView::enableHiddenFilter(bool enable)
{
    _proxy->enableHiddenFilter(enable);
}

void ProjectTreeView::setShowFilter(const QRegExp &regExp)
{
    _proxy->setShowFilter(regExp);
}

void ProjectTreeView::setShowFilter(const QString &pattern)
{
    setShowFilter(QRegExp(pattern, Qt::CaseInsensitive, QRegExp::Wildcard));
}

void ProjectTreeView::remove()
{
    if (!hasFocus())
        return;

    QModelIndexList selection = selectionModel()->selectedIndexes();
    if (selection.isEmpty())
        return;

    if (selection.size() == 1)
    {
        const QModelIndex &indexFile = _proxy->mapToSource(selection.first());
        if (!indexFile.isValid())
            return;

        if (_project->projectItemModel()->isDir(indexFile))
        {
            if (QMessageBox::question(this, "Remove directory?", QString("Do you realy want to remove '%1'?")
                                     .arg(_project->projectItemModel()->fileName(indexFile))) == QMessageBox::Yes)
                _project->projectItemModel()->rmdir(indexFile);
        }
        else if (_project->projectItemModel()->isFile(indexFile))
        {
            if (QMessageBox::question(this, "Remove file?", QString("Do you realy want to remove '%1'?")
                                      .arg(_project->projectItemModel()->fileName(indexFile))) == QMessageBox::Yes)
                _project->projectItemModel()->remove(indexFile);
        }
    }
    else
    {
        if (QMessageBox::question(this, "Remove directory?", QString("Do you realy want to remove theses %1 files?")
                                 .arg(selection.size())) != QMessageBox::Yes)
            return;
        QList<QPersistentModelIndex> pindex;
        foreach (QModelIndex selected, selection)
        {
            const QModelIndex &indexFile = _proxy->mapToSource(selected);
            if (!indexFile.isValid())
                continue;

            pindex.append(indexFile);
        }
        selectionModel()->clearSelection();
        foreach (QPersistentModelIndex index, pindex)
        {
            if (_project->projectItemModel()->isDir(index))
                _project->projectItemModel()->rmdir(index);
            else if (_project->projectItemModel()->isFile(index))
                _project->projectItemModel()->remove(index);
        }
    }
}

void ProjectTreeView::contextMenuEvent(QContextMenuEvent *event)
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
    if (_project->projectItemModel()->isDir(indexFile))
    {
        fileCreateAction = menu.addAction("Add new file here");
        fileCreateAction->setShortcut(QKeySequence::Delete);
    }
    QAction *fileRenameAction = menu.addAction("Rename");
    fileRenameAction->setShortcut(QKeySequence(Qt::Key_F2));
    QAction *dirRemoveAction = nullptr, *openDirAction = nullptr;
    if (_project->projectItemModel()->isDir(indexFile))
    {
        dirRemoveAction = menu.addAction("Remove directory");
        dirRemoveAction->setShortcut(QKeySequence::Delete);

        openDirAction = menu.addAction("Open directory as project");
    }
    else
    {
        menu.addAction(_removeAction);
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
    if (trigered == nullptr)
        return;

    // analyse clicked menu
    if (trigered == fileCreateAction)
    {
        QString fileName = QInputDialog::getText(this, "New file name", "Enter a name for this new file");
        QString filePath = _project->projectItemModel()->filePath(indexFile) + "/" + fileName;
        QFile file(filePath);
        if (!file.exists())
        {
            if (!file.open(QIODevice::WriteOnly))
                return;
            file.close();
            emit openedFile(filePath);
        }
    }
    else if (trigered == fileRenameAction)
        edit(index);
    else if (trigered == openDirAction)
    {
        QString path  =_project->projectItemModel()->filePath(indexFile);
        QProcess::startDetached(qApp->arguments()[0], QStringList()<<path);
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

void ProjectTreeView::mouseReleaseEvent(QMouseEvent *event)
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

void ProjectTreeView::mouseDoubleClickEvent(QMouseEvent *event)
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

void ProjectTreeView::keyPressEvent(QKeyEvent *event)
{
    QTreeView::keyPressEvent(event);

    if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return )
    {
        if (!hasFocus())
            return;

        const QModelIndexList indexList = selectionModel()->selectedIndexes();

        foreach (const QModelIndex &index, indexList)
        {
            if (!index.isValid())
                return;

            const QModelIndex &indexFile = _proxy->mapToSource(index);
            if (!_project->projectItemModel()->isDir(indexFile))
                emit openedFile(_project->projectItemModel()->filePath(indexFile));
        }
    }
}
