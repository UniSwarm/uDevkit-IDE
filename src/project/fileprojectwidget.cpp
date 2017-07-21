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

    _proxy = new FileProjectProxyModel();
    _proxy->setDynamicSortFilter(true);
    _proxy->setSourceModel(_project->fileItemModel());
    _proxy->setFilterRegExp(QRegExp("(nbproject|bin|build)", Qt::CaseInsensitive, QRegExp::RegExp));
    _proxy->setFilterKeyColumn(0);
    _proxy->setFilterRole(QFileSystemModel::FilePathRole);

    _fileView->setModel(_proxy);
    _fileView->setRootIndex(_proxy->mapFromSource(_project->fileItemModel()->index(project->rootPath())));
    for (int i = 1; i < _project->fileItemModel()->columnCount(); ++i)
        _fileView->hideColumn(i);
    connect(_fileView, &QTreeView::doubleClicked, this, &FileProjectWidget::openIndex);

    layout->addWidget(_fileView);
    setLayout(layout);
    _fileView->header()->close();
}

void FileProjectWidget::openIndex(const QModelIndex &index)
{
    emit doubleClickFile(_project->fileItemModel()->filePath(_proxy->mapToSource(index)));
}
