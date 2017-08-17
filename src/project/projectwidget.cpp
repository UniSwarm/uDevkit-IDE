#include "projectwidget.h"

#include <QBoxLayout>
#include <QDebug>
#include <QApplication>

ProjectWidget::ProjectWidget(Project *project, QWidget *parent) : QWidget(parent)
{
    _project = project;

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setMargin(0);

    _filterEdit = new QLineEdit();
    layout->addWidget(_filterEdit);

    _fileView = new ProjectTreeView(_project, this);
    //_fileView->proxy()->setHiddenFilter(QRegExp("(nbproject|bin|.*build.*|.*\\.git$|rtsim)", Qt::CaseInsensitive, QRegExp::RegExp));

    _fileView->setStyleSheet("QTreeView { selection-background-color: transparent; }");
    layout->addWidget(_fileView);
    setLayout(layout);

    connect(_fileView, &ProjectTreeView::openedFile, this, &ProjectWidget::openFile);
    connect(_fileView, &ProjectTreeView::closedFile, this, &ProjectWidget::closeFile);

    connect(_filterEdit, SIGNAL(textChanged(QString)), _fileView->proxy(), SLOT(setShowFilter(QString)));
}

void ProjectWidget::selectFile(const QString &fileName)
{
    _fileView->selectFile(fileName);
}

void ProjectWidget::openFile(const QString &fileName)
{
    emit openedFile(fileName);
}

void ProjectWidget::closeFile(const QString &fileName)
{
    emit closedFile(fileName);
}
