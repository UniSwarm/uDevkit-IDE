#include "fileprojectwidget.h"

#include <QBoxLayout>
#include <QHeaderView>
#include <QDebug>
#include <QApplication>

FileProjectWidget::FileProjectWidget(Project *project, QWidget *parent) : QWidget(parent)
{
    _project = project;

    QHBoxLayout *layout = new QHBoxLayout();
    layout->setMargin(0);

    _fileView = new QTreeView(this);

    _proxy = new FileProjectProxyModel(project);
    _proxy->setSourceModel(_project->fileItemModel());
    _proxy->setHiddenFilter(QRegExp("(nbproject|bin|.*build.*|.*\\.git$|rtsim)", Qt::CaseInsensitive, QRegExp::RegExp));

    //_proxy->setShowFilter(QRegExp("readme", Qt::CaseInsensitive, QRegExp::RegExp));

    _fileView->setModel(_proxy);
    _fileView->setRootIndex(_proxy->mapFromSource(_project->fileItemModel()->index(project->rootPath())));
    for (int i = 1; i < _project->fileItemModel()->columnCount(); ++i)
        _fileView->hideColumn(i);
    connect(_fileView, &QTreeView::doubleClicked, this, &FileProjectWidget::openIndex);
    _fileView->setStyleSheet("QTreeView { selection-background-color: transparent; }");
    layout->addWidget(_fileView);
    setLayout(layout);
    _fileView->header()->close();
}

void FileProjectWidget::openIndex(const QModelIndex &index)
{
    const QModelIndex &indexFile = _proxy->mapToSource(index);
    if (_project->fileItemModel()->isDir(indexFile))
        return;
    emit doubleClickFile(_project->fileItemModel()->filePath(indexFile));
}
