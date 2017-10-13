#include "mainwindow.h"

#include <QAction>
#include <QApplication>
#include <QBoxLayout>
#include <QDebug>
#include <QDesktopWidget>
#include <QFileDialog>
#include <QSettings>
#include <QSplitter>
#include <QMenuBar>
#include <QThread>
#include <QMessageBox>

const int MainWindow::MaxOldProject = 8;

MainWindow::MainWindow(Project *project, QWidget *parent) :
    QMainWindow(parent), _project(project)
{
    QString path;
    if(!_project)
        _project = new Project(QDir::home().canonicalPath());
    else
        path = _project->rootPath();

    _editorTabWidget = new EditorTabWidget(_project);

    setCentralWidget(_editorTabWidget);
    connect(_editorTabWidget, &EditorTabWidget::editorChange, this, &MainWindow::updateTitle);
    connect(_editorTabWidget, &EditorTabWidget::currentEditorModified, this, &MainWindow::updateTitle);

    createDocks();
    createMenus();

    readSettings();
    if (!path.isEmpty())
    {
        _oldProjects.removeOne(path);
        _oldProjects.prepend(path);
    }
    updateOldProjects();
}

MainWindow::~MainWindow()
{
    delete _project;
}

void MainWindow::createDocks()
{
    setCorner(Qt::TopLeftCorner, Qt::LeftDockWidgetArea);
    setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);
    setTabPosition(Qt::LeftDockWidgetArea, QTabWidget::North);

    _fileProjectDock = new QDockWidget(tr("project"), this);
    QWidget *fileProjectContent = new QWidget(_fileProjectDock);
    QLayout *fileProjectLayout = new QVBoxLayout();
    _fileProjectWidget = new ProjectWidget(_project);
    fileProjectLayout->addWidget(_fileProjectWidget);
    connect(_fileProjectWidget, &ProjectWidget::openedFile, _editorTabWidget, &EditorTabWidget::openFileEditor);
    connect(_fileProjectWidget, &ProjectWidget::closedFile, _editorTabWidget, &EditorTabWidget::closeFileEditor);
    connect(_editorTabWidget, &EditorTabWidget::currentFileChanged, _fileProjectWidget, &ProjectWidget::selectFile);
    fileProjectContent->setLayout(fileProjectLayout);
    _fileProjectDock->setWidget(fileProjectContent);
    addDockWidget(Qt::LeftDockWidgetArea, _fileProjectDock);

    _logDock = new QDockWidget(tr("log"), this);
    QWidget *logContent = new QWidget(_logDock);
    QLayout *logLayout = new QVBoxLayout();
    _logWidget = new LogWidget(_project);
    _logWidget->document()->setDefaultStyleSheet("p{margin: 0;}");
    logLayout->addWidget(_logWidget);
    logContent->setLayout(logLayout);
    _logDock->setWidget(logContent);
    connect(_logWidget, &LogWidget::openFileRequested, _editorTabWidget, &EditorTabWidget::openFileEditor);
    addDockWidget(Qt::BottomDockWidgetArea, _logDock);

    _searchReplaceDock = new QDockWidget(tr("search / replace"), this);
    QWidget *searchReplaceContent = new QWidget(_searchReplaceDock);
    QLayout *searchReplaceLayout = new QVBoxLayout();
    _searchReplaceWidget = new SearchReplaceWidget();
    searchReplaceLayout->addWidget(_searchReplaceWidget);
    connect(_editorTabWidget, &EditorTabWidget::editorChange, _searchReplaceWidget, &SearchReplaceWidget::setEditor);
    searchReplaceContent->setLayout(searchReplaceLayout);
    _searchReplaceDock->setWidget(searchReplaceContent);
    addDockWidget(Qt::BottomDockWidgetArea, _searchReplaceDock);
}

void MainWindow::createMenus()
{
    setMenuBar(new QMenuBar(this));
    //addToolBar(new QToolBar(this));

    // ============= Project =============
    QMenu *projectMenu = menuBar()->addMenu(tr("&Project"));

    QAction *openDirAction = new QAction(tr("Open &project"),this);
    openDirAction->setStatusTip(tr("Opens a project as directory"));
    projectMenu->addAction(openDirAction);
    connect(openDirAction, SIGNAL(triggered()), this, SLOT(openDir()));

    QAction *openFilesAction = new QAction(tr("&Open files"),this);
    openFilesAction->setStatusTip(tr("Opens files"));
    openFilesAction->setShortcut(QKeySequence::Open);
    projectMenu->addAction(openFilesAction);
    connect(openFilesAction, SIGNAL(triggered()), this, SLOT(openFiles()));

    projectMenu->addSeparator();
    for (int i=0; i<MaxOldProject; i++)
    {
        QAction *recentAction = new QAction(this);
        projectMenu->addAction(recentAction);
        recentAction->setVisible(false);
        connect(recentAction, SIGNAL(triggered()), this, SLOT(openRecentFile()));
        _oldProjectsActions.append(recentAction);
    }

    projectMenu->addSeparator();
    QAction *exit = new QAction(tr("E&xit"),this);
    exit->setStatusTip(tr("Exits RtIDE"));
    exit->setShortcut(QKeySequence::Quit);
    projectMenu->addAction(exit);
    connect(exit, SIGNAL(triggered()), this, SLOT(close()));

    QAction *action;
    action = new QAction(tr("git"), this);
    action->setShortcut(QKeySequence("F4"));
    addAction(action);
    connect(action, &QAction::triggered, this, &MainWindow::git);
    action = new QAction(tr("make"), this);
    action->setShortcut(QKeySequence("Ctrl+R"));
    addAction(action);
    connect(action, &QAction::triggered, this, &MainWindow::makeall);
    action = new QAction(tr("makeprog"), this);
    action->setShortcut(QKeySequence("Ctrl+T"));
    addAction(action);
    connect(action, &QAction::triggered, this, &MainWindow::makeprog);
    action = new QAction(tr("makeclean"), this);
    action->setShortcut(QKeySequence("Ctrl+E"));
    addAction(action);
    connect(action, &QAction::triggered, this, &MainWindow::makeclean);

    action = new QAction(tr("next tab"), this);
    action->setShortcut(QKeySequence::NextChild);
    addAction(action);
    connect(action, &QAction::triggered, _editorTabWidget, &EditorTabWidget::nextTab);

    action = new QAction(tr("search"), this);
    action->setShortcut(QKeySequence::QKeySequence::Find);
    addAction(action);
    connect(action, &QAction::triggered, _searchReplaceWidget, &SearchReplaceWidget::activate);

    // ============= Help =============
    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));

    QAction *aboutAction = new QAction(tr("&About"), this);
    connect(aboutAction, SIGNAL(triggered(bool)), this, SLOT(about()));
    helpMenu->addAction(aboutAction);

    QAction *aboutQtAction = new QAction(tr("About &Qt"), this);
    connect(aboutQtAction, SIGNAL(triggered(bool)), qApp, SLOT(aboutQt()));
    helpMenu->addAction(aboutQtAction);
}

bool MainWindow::openDir(const QString &path)
{
    QString mpath = path;
    if (mpath.isEmpty())
    {
        QFileDialog dialog(this);
        dialog.setOption(QFileDialog::DontUseNativeDialog);
        dialog.setFileMode(QFileDialog::Directory);
        if (!dialog.exec())
            return false;
        mpath = dialog.selectedFiles().first();
    }
    //_project->setRootPath(mpath);

    Project *project = new Project(mpath);
    MainWindow *w = new MainWindow(project);
    w->show();
    return true;
}

bool MainWindow::openFiles(const QStringList &paths)
{
    QStringList mpaths = paths;
    if (mpaths.isEmpty())
    {
        QFileDialog dialog(this);
        dialog.setOption(QFileDialog::DontUseNativeDialog);
        dialog.setFileMode(QFileDialog::ExistingFiles);
        if (!dialog.exec())
            return false;
        mpaths = dialog.selectedFiles();
    }
    foreach (QString apath, mpaths)
        _editorTabWidget->openFileEditor(apath);
    return true;
}

void MainWindow::git()
{
    _logWidget->start("git", QStringList()<<"diff"<<_project->rootDir().relativeFilePath(_editorTabWidget->currentFilePath()));
}

void MainWindow::makeall()
{
    _editorTabWidget->saveAllEditors();
    _logWidget->start("make", QStringList()<<"all"<<"-j"<<QString::number(QThread::idealThreadCount()));
}

void MainWindow::makeprog()
{
    _editorTabWidget->saveAllEditors();
    _logWidget->start("make", QStringList()<<"prog"<<"-j"<<QString::number(QThread::idealThreadCount()));
}

void MainWindow::makeclean()
{
    _logWidget->start("make", QStringList()<<"clean");
}

void MainWindow::updateTitle(Editor *editor)
{
    if(!editor)
        return;
    QString title = editor->filePath();
    if (editor->isModified())
        title.append("*");
    title.append(" | rtide");
    setWindowTitle(title);
}

bool MainWindow::event(QEvent *event)
{
    if(event->type()==QEvent::Close)
    {
        if(_editorTabWidget->closeAllEditors() < 0)
        {
            event->ignore();
            return false;
        }
        writeSettings();
    }
    return QMainWindow::event(event);
}

void MainWindow::updateOldProjects()
{
    for (int i=0; i<_oldProjects.size() && i < MaxOldProject; i++)
    {
        QString path = _oldProjects[i];
        _oldProjectsActions[i]->setVisible(true);
        _oldProjectsActions[i]->setData(path);
        _oldProjectsActions[i]->setText(QString("&%1. %2").arg(i+1).arg(path));
        _oldProjectsActions[i]->setStatusTip(tr("Open recent project '")+path+"'");
    }
}

void MainWindow::openRecentFile()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action)
        openDir(action->data().toString());
}

void MainWindow::writeSettings()
{
    QSettings settings("Robotips", "RtIDE");

    // MainWindow position/size/maximized
    settings.beginGroup("MainWindow");
    settings.setValue("size", normalGeometry().size());
    settings.setValue("pos", normalGeometry().topLeft());
    settings.setValue("maximized", isMaximized());
    settings.endGroup();

    // old projects write
    settings.beginWriteArray("projects");
    for (int i = 0; i < _oldProjects.size() && i < MaxOldProject; ++i)
    {
        settings.setArrayIndex(i);
        QString path = _oldProjects[i];
        settings.setValue("path", path);
    }
    settings.endArray();
}

void MainWindow::readSettings()
{
    QSettings settings("Robotips", "RtIDE");

    // MainWindow position/size/maximized
    settings.beginGroup("MainWindow");
    resize(settings.value("size", QSize(800, 600)).toSize());
    move(settings.value("pos", QPoint(200, 200)).toPoint());
    if(settings.value("maximized", true).toBool())
        showMaximized();
    settings.endGroup();

    // old projects read
    int size = settings.beginReadArray("projects");
    for (int i = 0; i < size && i < MaxOldProject; ++i)
    {
        settings.setArrayIndex(i);
        QString path = settings.value("path", "").toString();
        if(!_oldProjects.contains(path) && !path.isEmpty())
            _oldProjects.append(path);
    }
    settings.endArray();

    updateOldProjects();
}

void MainWindow::about()
{
    QMessageBox::about(this, "RtIDE v0", QString("Copyright (C) 2017 Robotips (<a href=\"https://robotips.fr\">robotips.fr</a>)<br>\
<br>\
This sofware is part of RtIDE distribution. To check for new version, please visit <a href=\"https://github.com/Robotips/rtide\">github.com/Robotips/rtide</a><br>\
<br>\
Written by <a href=\"https://github.com/sebcaux\">Sébastien CAUX (sebcaux)</a><br>\
<br>\
RtIDE is a free software: you can redistribute it and/or modify \
it under the terms of the GNU General Public License as published by \
the Free Software Foundation, either version 3 of the License, or \
(at your option) any later version.<br>\
<br>\
This program is distributed in the hope that it will be useful, \
but WITHOUT ANY WARRANTY; without even the implied warranty of \
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the \
GNU General Public License for more details.<br>\
<br>\
You should have received a copy of the GNU General Public License \
along with this program. If not, see <a href=\"http://www.gnu.org/licenses/\">www.gnu.org/licenses</a><br>\
<br>\
Build date: ") + __DATE__ + QString(" time: ")+__TIME__);
}
