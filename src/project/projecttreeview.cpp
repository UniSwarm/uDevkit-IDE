/**
 ** This file is part of the uDevkit-IDE project.
 ** Copyright 2017-2021 UniSwarm
 **
 ** This program is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU General Public License as published by
 ** the Free Software Foundation, either version 3 of the License, or
 ** (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 ** GNU General Public License for more details.
 **
 ** You should have received a copy of the GNU General Public License
 ** along with this program. If not, see <http://www.gnu.org/licenses/>.
 **/

#include "projecttreeview.h"

#include <QApplication>
#include <QDateTime>
#include <QDebug>
#include <QHeaderView>
#include <QInputDialog>
#include <QItemSelectionModel>
#include <QMenu>
#include <QMessageBox>
#include <QMouseEvent>
#include <QProcess>
#include <QProxyStyle>

#include "fileprojectinfo.h"
#include "mainwindow.h"

#include "project/projectitemmodel.h"
#include "projecttreeviewitemdelegate.h"
#include "versioncontrol/projectversioncontrol.h"

#include <settings/settingsmanager.h>

ProjectTreeView::ProjectTreeView(Project *project, QWidget *parent)
    : QTreeView(parent)
    , _project(project)
{
    _proxy = new ProjectItemProxyModel(project);
    _proxy->setSourceModel(_project->projectItemModel());
    setHiddenFilter(QRegularExpression(""));
    setIconSize(QSize(20, 20));
    _proxy->sort(0, Qt::AscendingOrder);

    setSelectionMode(QAbstractItemView::ExtendedSelection);

    setModel(_proxy);
    setEditTriggers(QAbstractItemView::EditKeyPressed);
    setStyleSheet("QTreeView { selection-background-color: transparent; }");
    header()->close();
    header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    header()->setStretchLastSection(false);
    setItemDelegate(new ProjectTreeViewItemDelegate());

    _removeAction = new QAction(this);
    _removeAction->setText(tr("Remove"));
    _removeAction->setShortcut(QKeySequence::Delete);
    _removeAction->setShortcutContext(Qt::WidgetShortcut);
    connect(_removeAction, &QAction::triggered, this, &ProjectTreeView::remove);
    addAction(_removeAction);

    _fileRenameAction = new QAction(this);
    _fileRenameAction->setText(tr("Rename"));
    _fileRenameAction->setShortcut(QKeySequence(Qt::Key_F2));
    connect(_fileRenameAction, &QAction::triggered, this, &ProjectTreeView::rename);
    addAction(_fileRenameAction);

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
    {
        return;
    }

    QModelIndex proxyIndex = _proxy->mapFromSource(fileIndex);
    if (!proxyIndex.isValid())
    {
        return;
    }

    selectionModel()->setCurrentIndex(proxyIndex, QItemSelectionModel::ClearAndSelect);
}

void ProjectTreeView::setHiddenFilter(const QRegularExpression &regExp)
{
    if (!regExp.isValid() || regExp.pattern().isEmpty())
    {
        _proxy->setHiddenFilter(QRegularExpression("^$"));
    }
    else
    {
        _proxy->setHiddenFilter(regExp);
    }
}

void ProjectTreeView::setHiddenFilter(const QString &pattern)
{
    setHiddenFilter(QRegularExpression(QRegularExpression::wildcardToRegularExpression(pattern), QRegularExpression::CaseInsensitiveOption));
}

void ProjectTreeView::enableHiddenFilter(bool enable)
{
    _proxy->enableHiddenFilter(enable);
}

void ProjectTreeView::setShowFilter(const QRegularExpression &regExp)
{
    _proxy->setShowFilter(regExp);
}

void ProjectTreeView::setShowFilter(const QString &pattern)
{
    setShowFilter(QRegularExpression(QRegularExpression::wildcardToRegularExpression(pattern), QRegularExpression::CaseInsensitiveOption));
}

void ProjectTreeView::remove()
{
    if (!hasFocus())
    {
        return;
    }

    QModelIndexList selection = selectionModel()->selectedIndexes();
    if (selection.isEmpty())
    {
        return;
    }

    if (selection.size() == 1)
    {
        const QModelIndex &indexFile = _proxy->mapToSource(selection.first());
        if (!indexFile.isValid())
        {
            return;
        }

        if (_project->projectItemModel()->isDir(indexFile))
        {
            if (QMessageBox::question(this, tr("Remove directory?"), tr("Do you realy want to remove '%1'?").arg(_project->projectItemModel()->fileName(indexFile)))
                == QMessageBox::Yes)
            {
                _project->projectItemModel()->rmdir(indexFile);
            }
        }
        else if (_project->projectItemModel()->isFile(indexFile))
        {
            if (QMessageBox::question(this, tr("Remove file?"), tr("Do you realy want to remove '%1'?").arg(_project->projectItemModel()->fileName(indexFile))) == QMessageBox::Yes)
            {
                _project->projectItemModel()->remove(indexFile);
            }
        }
    }
    else
    {
        if (QMessageBox::question(this, tr("Remove directory?"), tr("Do you realy want to remove theses %1 files?").arg(selection.size())) != QMessageBox::Yes)
        {
            return;
        }
        QList<QPersistentModelIndex> pindex;
        for (const QModelIndex &selected : selection)
        {
            const QModelIndex &indexFile = _proxy->mapToSource(selected);
            if (!indexFile.isValid())
            {
                continue;
            }

            pindex.append(indexFile);
        }
        selectionModel()->clearSelection();
        for (const QPersistentModelIndex &index : pindex)
        {
            if (_project->projectItemModel()->isDir(index))
            {
                _project->projectItemModel()->rmdir(index);
            }
            else if (_project->projectItemModel()->isFile(index))
            {
                _project->projectItemModel()->remove(index);
            }
        }
    }
}

void ProjectTreeView::rename()
{
    QModelIndexList selection = selectionModel()->selectedIndexes();
    if (selection.isEmpty())
    {
        return;
    }

    if (selection.size() != 1)
    {
        return;
    }

    edit(selection.first());
}

void ProjectTreeView::contextMenuEvent(QContextMenuEvent *event)
{
    const QModelIndex &index = indexAt(event->pos());
    if (!index.isValid())
    {
        return;
    }
    const QModelIndex &indexFile = _proxy->mapToSource(index);
    if (!indexFile.isValid())
    {
        return;
    }

    QMenu menu;

    // file commands
    QAction *fileCreateAction = nullptr;
    if (_project->projectItemModel()->isDir(indexFile))
    {
        fileCreateAction = menu.addAction(tr("Add new file here"));
        // fileCreateAction->setShortcut(QKeySequence::New);
    }
    menu.addAction(_fileRenameAction);
    QAction *openDirAction = nullptr;
    QAction *openTermDirAction = nullptr;
    if (_project->projectItemModel()->isDir(indexFile))
    {
        _removeAction->setText(tr("Remove directory"));
        menu.addAction(_removeAction);

        openDirAction = menu.addAction(tr("Open directory as project"));
        openTermDirAction = menu.addAction(tr("Open in terminal"));
    }
    else
    {
        _removeAction->setText(tr("Remove"));
        menu.addAction(_removeAction);
    }

    // git commands
    QAction *versionValidAction = nullptr;
    QAction *versionInvalidAction = nullptr;
    QAction *versionCheckoutAction = nullptr;
    FileProjectInfo info(_project->projectItemModel()->filePath(indexFile), _project);
    if (_project->versionControl()->isValid())
    {
        menu.addSeparator();
        if (!info.isTracked() || info.isModified())
        {
            versionValidAction = menu.addAction(_project->versionControl()->versionControlName() + " add");
        }
        if (info.isValidated())
        {
            versionInvalidAction = menu.addAction(_project->versionControl()->versionControlName() + " reset HEAD");
        }
        if (info.isModified())
        {
            versionCheckoutAction = menu.addAction(_project->versionControl()->versionControlName() + " checkout");
        }
    }

    // exec menu
    QAction *trigered = menu.exec(event->globalPos());
    if (trigered == nullptr)
    {
        return;
    }

    // analyse clicked menu
    if (trigered == fileCreateAction)
    {
        QString fileName = QInputDialog::getText(this, tr("New file name"), tr("Enter a name for this new file"));
        QString filePath = _project->projectItemModel()->filePath(indexFile) + "/" + fileName;
        QFile file(filePath);
        if (!file.exists())
        {
            if (!file.open(QIODevice::WriteOnly))
            {
                return;
            }

            if (fileName.endsWith(".h") || fileName.endsWith(".c"))
            {
                QTextStream stream(&file);
                stream << "/**\n * @file " << fileName << "\n * @author " << SettingsManager::userName() << " (" << SettingsManager::userPseudo() << ")"
                       << "\n * @copyright UniSwarm " << QDateTime::currentDateTime().date().year() << "\n *\n * @date "
                       << QLocale::c().toString(QDateTime::currentDateTime(), "MMMM d, yyyy, hh:mm AP") << "\n *\n * @brief .....\n */\n\n";

                if (fileName.endsWith(".h"))
                {
                    QString define = fileName.replace(".", "_").toUpper();
                    stream << "#ifndef " << define << "\n"
                           << "#define " << define << "\n"
                           << "\n\n\n"
                           << "#endif  // " << define << "\n";
                }
                if (fileName.endsWith(".c"))
                {
                    stream << "#include \"" << fileName.replace(".c", ".h") << "\"\n\n";
                }
            }

            file.close();
            emit openedFile(filePath);
        }
    }
    else if (trigered == openDirAction)
    {
        QString path = _project->projectItemModel()->filePath(indexFile);
        QProcess::startDetached(qApp->arguments()[0], QStringList() << path);
    }
    else if (trigered == openTermDirAction)
    {
        QString path = _project->projectItemModel()->filePath(indexFile);
#if defined(Q_OS_LINUX)
        QProcess::startDetached("gnome-terminal", QStringList() << "--working-directory=" + path);  // TODO make it work on all platforms
#elif defined(Q_OS_WIN)
        QProcess::startDetached("mintty.exe", QStringList() << "--dir" << path);
#endif
    }
    else if (trigered == versionValidAction)
    {
        _project->versionControl()->validFile(QSet<QString>() << info.filePath());
    }
    else if (trigered == versionInvalidAction)
    {
        _project->versionControl()->inValidFile(QSet<QString>() << info.filePath());
    }
    else if (trigered == versionCheckoutAction)
    {
        if (QMessageBox::question(
                this,
                tr("Checkout file?"),
                QString(tr("Do you realy want to checkout '%1'?\nIt will be restored to the last valid state.")).arg(_project->projectItemModel()->fileName(indexFile)))
            == QMessageBox::Yes)
        {
            _project->versionControl()->checkoutFile(QSet<QString>() << info.filePath());
        }
    }
}

void ProjectTreeView::mouseReleaseEvent(QMouseEvent *event)
{
    QTreeView::mouseReleaseEvent(event);

    if (event->button() == Qt::MiddleButton)
    {
        const QPersistentModelIndex index = indexAt(event->pos());
        if (!index.isValid())
        {
            return;
        }

        const QModelIndex &indexFile = _proxy->mapToSource(index);
        emit closedFile(_project->projectItemModel()->filePath(indexFile));
    }
}

void ProjectTreeView::mouseDoubleClickEvent(QMouseEvent *event)
{
    QTreeView::mouseDoubleClickEvent(event);

    if (!event->buttons().testFlag(Qt::LeftButton))
    {
        return;
    }

    const QPersistentModelIndex index = indexAt(event->pos());
    if (!index.isValid())
    {
        return;
    }

    const QModelIndex &indexFile = _proxy->mapToSource(index);
    if (!_project->projectItemModel()->isDir(indexFile))
    {
        emit openedFile(_project->projectItemModel()->filePath(indexFile));
    }
}

void ProjectTreeView::keyPressEvent(QKeyEvent *event)
{
    QTreeView::keyPressEvent(event);

    if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
    {
        if (!hasFocus())
        {
            return;
        }

        QStringList filesPathToOpen;

        const QModelIndexList indexList = selectionModel()->selectedIndexes();
        for (const QModelIndex &index : indexList)
        {
            if (!index.isValid())
            {
                return;
            }

            const QModelIndex &indexFile = _proxy->mapToSource(index);
            if (!_project->projectItemModel()->isDir(indexFile))
            {
                filesPathToOpen.append(_project->projectItemModel()->filePath(indexFile));
            }
        }

        for (const QString &path : filesPathToOpen)
        {
            emit openedFile(path);
        }
    }
}
