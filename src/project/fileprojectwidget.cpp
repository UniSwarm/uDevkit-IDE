#include "fileprojectwidget.h"

#include <QBoxLayout>
#include <QHeaderView>
#include <QDebug>
#include <QApplication>

FileProjectWidget::FileProjectWidget(Project *project, QWidget *parent) : QWidget(parent)
{
    _project = project;

    QHBoxLayout *layout = new QHBoxLayout();

    _fileView = new QTreeView(this);
    _fileItemModel = new QFileSystemModel();
    _fileItemModel->setRootPath(QApplication::applicationDirPath()+"/../../rtprog/");

    _proxy = new FileProjectProxyModel(_project);
    _proxy->setDynamicSortFilter(true);
    _proxy->setSourceModel(_fileItemModel);
    _proxy->setFilterRegExp(QRegExp("(nbproject|bin|build)", Qt::CaseInsensitive, QRegExp::RegExp));
    _proxy->setFilterKeyColumn(0);
    _proxy->setFilterRole(QFileSystemModel::FilePathRole);

    _fileView->setModel(_proxy);
    _fileView->setRootIndex(_proxy->mapFromSource(_fileItemModel->index(QApplication::applicationDirPath()+"/../../rtprog/")));
    for (int i = 1; i < _fileItemModel->columnCount(); ++i)
        _fileView->hideColumn(i);
    connect(_fileView, &QTreeView::doubleClicked, this, &FileProjectWidget::openIndex);

    layout->addWidget(_fileView);
    setLayout(layout);
    _fileView->header()->close();
}

void FileProjectWidget::openIndex(const QModelIndex &index)
{
    emit doubleClickFile(_fileItemModel->filePath(_proxy->mapToSource(index)));
}
