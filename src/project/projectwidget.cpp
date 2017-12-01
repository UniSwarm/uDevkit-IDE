#include "projectwidget.h"

#include <QAction>
#include <QBoxLayout>
#include <QDebug>
#include <QApplication>
#include <QMenu>

ProjectWidget::ProjectWidget(Project *project, QWidget *parent) : QWidget(parent)
{
    _project = project;

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setMargin(0);

    QHBoxLayout *filterLayout = new QHBoxLayout();
    filterLayout->setMargin(0);

    _filterEdit = new QLineEdit();
    _filterEdit->setClearButtonEnabled(true);
    filterLayout->addWidget(_filterEdit);
    _menuBtn = new QToolButton();
    _menuBtn->setText("...");
    _menuBtn->setPopupMode(QToolButton::InstantPopup);
    filterLayout->addWidget(_menuBtn);

    layout->addItem(filterLayout);

    _projectView = new ProjectTreeView(_project, this);
    _projectView->setHiddenFilter(QRegExp("(nbproject|bin|.*build.*|.*\\.git$|rtsim)", Qt::CaseInsensitive, QRegExp::RegExp));
    _projectView->proxy()->enableHiddenFilter();

    layout->addWidget(_projectView);
    setLayout(layout);

    connect(_projectView, &ProjectTreeView::openedFile, this, &ProjectWidget::openFile);
    connect(_projectView, &ProjectTreeView::closedFile, this, &ProjectWidget::closeFile);

    connect(_filterEdit, SIGNAL(textChanged(QString)), _projectView->proxy(), SLOT(setShowFilter(QString)));

    createMenu();
}

void ProjectWidget::selectFile(const QString &fileName)
{
    _projectView->selectFile(fileName);
}

void ProjectWidget::openFile(const QString &fileName)
{
    emit openedFile(fileName);
}

void ProjectWidget::closeFile(const QString &fileName)
{
    emit closedFile(fileName);
}

void ProjectWidget::createMenu()
{
    QMenu *menu = new QMenu();

    QAction *enFilter = menu->addAction("enable filter");
    enFilter->setCheckable(true);
    enFilter->setChecked(true);
    connect(enFilter, &QAction::toggled, _projectView->proxy(), &ProjectItemProxyModel::enableHiddenFilter);

    _menuBtn->setMenu(menu);
}
