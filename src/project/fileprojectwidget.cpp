#include "fileprojectwidget.h"

#include <QBoxLayout>
#include <QDebug>
#include <QApplication>

FileProjectWidget::FileProjectWidget(Project *project, QWidget *parent) : QWidget(parent)
{
    _project = project;

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setMargin(0);

    _filterEdit = new QLineEdit();
    layout->addWidget(_filterEdit);

    _fileView = new FileTreeView(_project, this);
    _fileView->proxy()->setHiddenFilter(QRegExp("(nbproject|bin|.*build.*|.*\\.git$|rtsim)", Qt::CaseInsensitive, QRegExp::RegExp));

    _fileView->setStyleSheet("QTreeView { selection-background-color: transparent; }");
    layout->addWidget(_fileView);
    setLayout(layout);

    connect(_fileView, &FileTreeView::doubleClickedDir, this, &FileProjectWidget::doubleClickDir);
    connect(_fileView, &FileTreeView::doubleClickedFile, this, &FileProjectWidget::doubleClickFile);

    connect(_filterEdit, SIGNAL(textChanged(QString)), _fileView->proxy(), SLOT(setShowFilter(QString)));
}

void FileProjectWidget::selectFile(const QString &fileName)
{
    _fileView->selectFile(fileName);
}

void FileProjectWidget::doubleClickDir(const QString &fileName)
{
    emit doubleClickedDir(fileName);
}

void FileProjectWidget::doubleClickFile(const QString &fileName)
{
    emit doubleClickedFile(fileName);
}
