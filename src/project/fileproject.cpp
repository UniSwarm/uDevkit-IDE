#include "fileproject.h"

#include <QBoxLayout>
#include <QHeaderView>
#include <QDebug>
#include <QApplication>

FileProject::FileProject(QWidget *parent) : QWidget(parent)
{
    QHBoxLayout *layout = new QHBoxLayout();

    _fileView = new QTreeView(this);
    _fileItemModel = new QFileSystemModel();
    _fileItemModel->setRootPath(QApplication::applicationDirPath()+"/../../rtprog/");

    _proxy = new FileProjectProxyModel();
    _proxy->setDynamicSortFilter(true);
    _proxy->setSourceModel(_fileItemModel);
    _proxy->setFilterRegExp(QRegExp("(nbproject|bin|build)", Qt::CaseInsensitive, QRegExp::RegExp));
    _proxy->setFilterKeyColumn(0);
    _proxy->setFilterRole(QFileSystemModel::FilePathRole);

    _fileView->setModel(_proxy);
    _fileView->setRootIndex(_proxy->mapFromSource(_fileItemModel->index(QApplication::applicationDirPath()+"/../../rtprog/")));
    for (int i = 1; i < _fileItemModel->columnCount(); ++i)
        _fileView->hideColumn(i);
    connect(_fileView, &QTreeView::doubleClicked, this, &FileProject::openIndex);

    layout->addWidget(_fileView);
    setLayout(layout);
    _fileView->header()->close();
}

void FileProject::openIndex(const QModelIndex &index)
{
    emit doubleClickFile(_fileItemModel->filePath(_proxy->mapToSource(index)));
}
