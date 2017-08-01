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

    connect(_fileView, &FileTreeView::openedFile, this, &FileProjectWidget::openFile);
    connect(_fileView, &FileTreeView::closedFile, this, &FileProjectWidget::closeFile);

    connect(_filterEdit, SIGNAL(textChanged(QString)), _fileView->proxy(), SLOT(setShowFilter(QString)));
}

void FileProjectWidget::selectFile(const QString &fileName)
{
    _fileView->selectFile(fileName);
}

void FileProjectWidget::openFile(const QString &fileName)
{
    emit openedFile(fileName);
}

void FileProjectWidget::closeFile(const QString &fileName)
{
    emit closedFile(fileName);
}
