#include "mainwindow.h"

#include <QAction>
#include <QApplication>
#include <QBoxLayout>
#include <QDebug>
#include <QDesktopWidget>
#include <QFileDialog>
#include <QSplitter>
#include <QMenuBar>
#include <QThread>

MainWindow::MainWindow(Project *project, QWidget *parent) :
    QMainWindow(parent), _project(project)
{
    if(!_project)
        _project = new Project(QDir::home().canonicalPath());

    _editorTabWidget = new EditorTabWidget(_project);

    setCentralWidget(_editorTabWidget);
    connect(_editorTabWidget, &EditorTabWidget::editorChange, this, &MainWindow::updateTitle);
    connect(_editorTabWidget, &EditorTabWidget::currentEditorModified, this, &MainWindow::updateTitle);
    resize(QDesktopWidget().availableGeometry(this).size() * 0.7);

    createDocks();
    createMenus();
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

    _fileProjectDock = new QDockWidget("project", this);
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

    _logDock = new QDockWidget("log", this);
    QWidget *logContent = new QWidget(_logDock);
    QLayout *logLayout = new QVBoxLayout();
    _logWidget = new LogWidget(_project);
    _logWidget->document()->setDefaultStyleSheet("p{margin: 0;}");
    logLayout->addWidget(_logWidget);
    logContent->setLayout(logLayout);
    _logDock->setWidget(logContent);
    connect(_logWidget, &LogWidget::openFileRequested, _editorTabWidget, &EditorTabWidget::openFileEditor);
    addDockWidget(Qt::BottomDockWidgetArea, _logDock);

    _searchReplaceDock = new QDockWidget("search / replace", this);
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

    QMenu *projectMenu = menuBar()->addMenu("&Project");

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

    projectMenu->addSeparator();
    QAction *exit = new QAction(tr("E&xit"),this);
    exit->setStatusTip(tr("Exits RtIDE"));
    exit->setShortcut(QKeySequence::Quit);
    projectMenu->addAction(exit);
    connect(exit, SIGNAL(triggered()), this, SLOT(close()));

    QAction *action;
    action = new QAction(QString("git"), this);
    action->setShortcut(QKeySequence("F4"));
    addAction(action);
    connect(action, &QAction::triggered, this, &MainWindow::git);
    action = new QAction(QString("make"), this);
    action->setShortcut(QKeySequence("Ctrl+R"));
    addAction(action);
    connect(action, &QAction::triggered, this, &MainWindow::makeall);

    action = new QAction(QString("next tab"), this);
    action->setShortcut(QKeySequence::NextChild);
    addAction(action);
    connect(action, &QAction::triggered, _editorTabWidget, &EditorTabWidget::nextTab);

    action = new QAction(QString("search"), this);
    action->setShortcut(QKeySequence::QKeySequence::Find);
    addAction(action);
    connect(action, &QAction::triggered, _searchReplaceWidget, &SearchReplaceWidget::activate);
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
    _logWidget->start("make", QStringList()<<"all"<<"-j"<<QString::number(QThread::idealThreadCount()));
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
    }
    return QMainWindow::event(event);
}
