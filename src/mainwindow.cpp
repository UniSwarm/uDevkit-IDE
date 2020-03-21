#include "mainwindow.h"

#include <QAction>
#include <QApplication>
#include <QBoxLayout>
#include <QDebug>
#include <QDesktopWidget>
#include <QFileDialog>
#include <QMenuBar>
#include <QMessageBox>
#include <QSettings>
#include <QSplitter>
#include <QStatusBar>
#include <QThread>

#include "settings/pages/settingswindow.h"
#include "ui/iconneddockstyle.h"

const int MainWindow::MaxOldProject = 8;

MainWindow::MainWindow(Project *project, QWidget *parent)
    : QMainWindow(parent)
    , _project(project)
{
    setWindowIcon(QIcon(":/icons/UNIdevkit.ico"));
    QString path;
    if (!_project)
    {
        _project = new Project();
    }
    else
    {
        path = _project->rootPath();
    }
    updateTitle();

    _editorTabWidget = new EditorTabWidget(_project);

    setCentralWidget(_editorTabWidget);
    connect(_editorTabWidget, &EditorTabWidget::editorChange, this, &MainWindow::updateTitle);
    connect(_editorTabWidget, &EditorTabWidget::editorChange, this, &MainWindow::updateAction);
    connect(_editorTabWidget, &EditorTabWidget::currentEditorModified, this, &MainWindow::updateTitle);
    connect(_editorTabWidget, &EditorTabWidget::statusChanged, this, &MainWindow::updateStatus);

    createDocks();
    createMenus();
    setStatusBar(new QStatusBar());

    setGeometry(100, 100, 800, 600);

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

    _fileProjectDock = new QDockWidget(tr("Project"), this);
    _fileProjectDock->setObjectName("fileProjectDock");
    QWidget *fileProjectContent = new QWidget(_fileProjectDock);
    QLayout *fileProjectLayout = new QVBoxLayout();
    _fileProjectWidget = new ProjectWidget(_project);
    fileProjectLayout->addWidget(_fileProjectWidget);
    connect(_fileProjectWidget, &ProjectWidget::openedFile, _editorTabWidget, &EditorTabWidget::openFileEditor);
    connect(_fileProjectWidget, &ProjectWidget::closedFile, _editorTabWidget, &EditorTabWidget::closeFileEditor);
    connect(_editorTabWidget, &EditorTabWidget::currentFileChanged, _fileProjectWidget, &ProjectWidget::selectFile);
    fileProjectContent->setLayout(fileProjectLayout);
    _fileProjectDock->setWidget(fileProjectContent);
    _fileProjectDock->setStyle(new IconnedDockStyle(QIcon(":/icons/img/dark/icons8-binder.png"), _fileProjectDock->style()));
    addDockWidget(Qt::LeftDockWidgetArea, _fileProjectDock);

    _logDock = new QDockWidget(tr("Log"), this);
    _logDock->setObjectName("logDock");
    QWidget *logContent = new QWidget(_logDock);
    QLayout *logLayout = new QVBoxLayout();
    _logWidget = new LogWidget(_project);
    _logWidget->document()->setDefaultStyleSheet("p{margin: 0;}");
    logLayout->addWidget(_logWidget);
    logContent->setLayout(logLayout);
    _logDock->setStyle(new IconnedDockStyle(QIcon(":/icons/img/dark/icons8-console.png"), _logDock->style()));
    connect(_logWidget, &LogWidget::openFileRequested, _editorTabWidget, &EditorTabWidget::openFileEditor);
    _logDock->setWidget(logContent);
    addDockWidget(Qt::BottomDockWidgetArea, _logDock);

    _searchReplaceDock = new QDockWidget(tr("Search / replace"), this);
    _searchReplaceDock->setObjectName("searchReplaceDock");
    QWidget *searchReplaceContent = new QWidget(_searchReplaceDock);
    QLayout *searchReplaceLayout = new QVBoxLayout();
    _searchReplaceWidget = new SearchReplaceWidget();
    searchReplaceLayout->addWidget(_searchReplaceWidget);
    _searchReplaceDock->setStyle(new IconnedDockStyle(QIcon(":/icons/img/dark/icons8-search-button.png"), _searchReplaceDock->style()));
    connect(_editorTabWidget, &EditorTabWidget::editorChange, _searchReplaceWidget, &SearchReplaceWidget::setEditor);
    searchReplaceContent->setLayout(searchReplaceLayout);
    _searchReplaceDock->setWidget(searchReplaceContent);
    addDockWidget(Qt::BottomDockWidgetArea, _searchReplaceDock);
}

void MainWindow::createMenus()
{
    setMenuBar(new QMenuBar(this));
    // addToolBar(new QToolBar(this));

    // ============= file =============
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));

    QAction *openDirAction = new QAction(tr("Open &project"), this);
    openDirAction->setIcon(QIcon(":/icons/img/dark/icons8-open-box.png"));
    openDirAction->setStatusTip(tr("Opens a project"));
    fileMenu->addAction(openDirAction);
    connect(openDirAction, SIGNAL(triggered()), this, SLOT(openDir()));

    QAction *openFilesAction = new QAction(tr("&Open files"), this);
    openFilesAction->setIcon(QIcon(":/icons/img/dark/icons8-open.png"));
    openFilesAction->setStatusTip(tr("Opens files"));
    openFilesAction->setShortcut(QKeySequence::Open);
    fileMenu->addAction(openFilesAction);
    connect(openFilesAction, SIGNAL(triggered()), this, SLOT(openFiles()));

    fileMenu->addSeparator();

    QAction *switchHeaderAction = new QAction(tr("&Switch header"), this);
    switchHeaderAction->setIcon(QIcon(":/icons/img/dark/icons8-switch.png"));
    switchHeaderAction->setStatusTip(tr("Switch between header and source"));
    switchHeaderAction->setShortcut(QKeySequence("F4"));
    fileMenu->addAction(switchHeaderAction);
    connect(switchHeaderAction, SIGNAL(triggered()), _editorTabWidget, SLOT(switchHeader()));

    fileMenu->addSeparator();
    for (int i = 0; i < MaxOldProject; i++)
    {
        QAction *recentAction = new QAction(this);
        fileMenu->addAction(recentAction);
        recentAction->setVisible(false);
        connect(recentAction, SIGNAL(triggered()), this, SLOT(openRecentFile()));
        _oldProjectsActions.append(recentAction);
    }

    fileMenu->addSeparator();
    QAction *exitAction = new QAction(tr("E&xit"), this);
    exitAction->setIcon(QIcon(":/icons/img/dark/icons8-quit.png"));
    exitAction->setStatusTip(tr("Exits uDevkit-IDE"));
    exitAction->setShortcut(QKeySequence::Quit);
    fileMenu->addAction(exitAction);
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    QAction *action;
    action = new QAction(tr("git"), this);
    action->setShortcut(QKeySequence("F5"));
    addAction(action);
    connect(action, &QAction::triggered, this, &MainWindow::git);

    // ============= Edit =============
    QMenu *editMenu = menuBar()->addMenu(tr("&Edit"));

    action = editMenu->addAction(tr("&Undo"));
    action->setIcon(QIcon(":/icons/img/dark/icons8-undo.png"));
    action->setShortcut(QKeySequence::Undo);
    action->setEnabled(false);
    connect(action, &QAction::triggered, _editorTabWidget, &EditorTabWidget::undo);
    connect(_editorTabWidget, &EditorTabWidget::undoAvailable, action, &QAction::setEnabled);

    action = editMenu->addAction(tr("&Redo"));
    action->setIcon(QIcon(":/icons/img/dark/icons8-redo.png"));
    action->setShortcut(QKeySequence::Redo);
    action->setEnabled(false);
    connect(action, &QAction::triggered, _editorTabWidget, &EditorTabWidget::redo);
    connect(_editorTabWidget, &EditorTabWidget::redoAvailable, action, &QAction::setEnabled);

    editMenu->addSeparator();

    action = editMenu->addAction(tr("Cu&t"));
    action->setIcon(QIcon(":/icons/img/dark/icons8-cut.png"));
    action->setShortcut(QKeySequence::Cut);
    action->setEnabled(false);
    connect(action, &QAction::triggered, _editorTabWidget, &EditorTabWidget::cut);
    connect(_editorTabWidget, &EditorTabWidget::copyAvailable, action, &QAction::setEnabled);

    action = editMenu->addAction(tr("&Copy"));
    action->setIcon(QIcon(":/icons/img/dark/icons8-copy.png"));
    action->setShortcut(QKeySequence::Copy);
    action->setEnabled(false);
    connect(action, &QAction::triggered, _editorTabWidget, &EditorTabWidget::copy);
    connect(_editorTabWidget, &EditorTabWidget::copyAvailable, action, &QAction::setEnabled);

    _pasteAction = editMenu->addAction(tr("&Paste"));
    _pasteAction->setIcon(QIcon(":/icons/img/dark/icons8-paste.png"));
    _pasteAction->setShortcut(QKeySequence::Paste);
    _pasteAction->setEnabled(false);
    connect(_pasteAction, &QAction::triggered, _editorTabWidget, &EditorTabWidget::paste);

    editMenu->addSeparator();

    _searchAction = editMenu->addAction(tr("&Search"));
    _searchAction->setIcon(QIcon(":/icons/img/dark/icons8-search-button.png"));
    _searchAction->setShortcut(QKeySequence::Find);
    _searchAction->setEnabled(false);
    connect(_searchAction, &QAction::triggered, _searchReplaceWidget, &SearchReplaceWidget::activateResearch);

    _replaceAction = editMenu->addAction(tr("&Replace"));
    _replaceAction->setIcon(QIcon(":/icons/img/dark/icons8-find-and-replace.png"));
    _replaceAction->setShortcut(QKeySequence::Replace);
    _replaceAction->setEnabled(false);
    connect(_replaceAction, &QAction::triggered, _searchReplaceWidget, &SearchReplaceWidget::activateReplace);

    editMenu->addSeparator();

    action = editMenu->addAction(tr("&Format text"));
    action->setStatusTip(tr("Removes end spaces, replaces tabs by spaces"));
    action->setShortcut(QKeySequence("Ctrl+D"));
    connect(action, &QAction::triggered, _editorTabWidget, &EditorTabWidget::format);

    action = editMenu->addAction(tr("&clang-format"));
    action->setStatusTip(tr("Call clang-format inplace"));
    action->setShortcut(QKeySequence("F6"));
    connect(action, &QAction::triggered, this, &MainWindow::launchFormat);

    editMenu->addSeparator();

    action = editMenu->addAction(tr("P&references"));
    action->setIcon(QIcon(":/icons/img/dark/icons8-settings.png"));
    action->setShortcut(QKeySequence::Preferences);
    action->setShortcut(QKeySequence("Ctrl+P"));
    connect(action, &QAction::triggered, this, &MainWindow::showSettings);

    // ============= Project =============
    QMenu *projectMenu = menuBar()->addMenu(tr("&Project"));

    action = projectMenu->addAction(tr("Clea&n"));
    action->setIcon(QIcon(":/icons/img/dark/icons8-broom.png"));
    action->setShortcut(QKeySequence("Ctrl+E"));
    connect(action, &QAction::triggered, this, &MainWindow::makeclean);

    action = projectMenu->addAction(tr("&Compile"));
    action->setIcon(QIcon(":/icons/img/dark/icons8-maintenance.png"));
    action->setShortcut(QKeySequence("Ctrl+R"));
    connect(action, &QAction::triggered, this, &MainWindow::makeall);

    action = projectMenu->addAction(tr("&Program"));
    action->setIcon(QIcon(":/icons/img/dark/icons8-software-installer.png"));
    action->setShortcut(QKeySequence("Ctrl+T"));
    connect(action, &QAction::triggered, this, &MainWindow::makeprog);

    action = projectMenu->addAction(tr("Compile &sim"));
    action->setIcon(QIcon(":/icons/img/dark/icons8-test-tube.png"));
    action->setShortcut(QKeySequence("Ctrl+G"));
    connect(action, &QAction::triggered, this, &MainWindow::makesim);

    // ============= View =============
    QMenu *viewMenu = menuBar()->addMenu(tr("&View"));

    action = _logDock->toggleViewAction();
    action->setText(tr("        &Log"));
    action->setIcon(QIcon(":/icons/img/dark/icons8-console.png"));
    viewMenu->addAction(action);

    action = _fileProjectDock->toggleViewAction();
    action->setText(tr("        &Project file"));
    action->setIcon(QIcon(":/icons/img/dark/icons8-binder.png"));
    viewMenu->addAction(action);

    action = _searchReplaceDock->toggleViewAction();
    action->setText(tr("        &Search/replace"));
    action->setIcon(QIcon(":/icons/img/dark/icons8-search-button.png"));
    viewMenu->addAction(action);

    // ============= Help =============
    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));

    QAction *aboutAction = new QAction(tr("&About"), this);
    aboutAction->setIcon(QIcon(":/icons/img/dark/icons8-about.png"));
    connect(aboutAction, SIGNAL(triggered(bool)), this, SLOT(about()));
    helpMenu->addAction(aboutAction);

    QAction *aboutQtAction = new QAction(tr("About &Qt"), this);
    aboutQtAction->setIcon(QIcon(":/icons/img/dark/icons8-system-information.png"));
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
        if (_editorTabWidget->currentEditor())
        {
            QFileInfo file(_editorTabWidget->currentEditor()->filePath());
            dialog.setDirectory(file.absolutePath());
        }
        else
        {
            dialog.setDirectory(_project->rootPath());
        }
        if (!dialog.exec())
        {
            return false;
        }
        mpath = dialog.selectedFiles().first();
    }
    _project->setRootPath(mpath);

    _oldProjects.removeOne(mpath);
    _oldProjects.prepend(mpath);
    updateOldProjects();

    updateTitle();

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
        if (_editorTabWidget->currentEditor())
        {
            QFileInfo file(_editorTabWidget->currentEditor()->filePath());
            dialog.setDirectory(file.absolutePath());
        }
        else
        {
            dialog.setDirectory(_project->rootPath());
        }
        if (!dialog.exec())
        {
            return false;
        }
        mpaths = dialog.selectedFiles();
    }
    foreach (QString apath, mpaths)
    {
        _editorTabWidget->openFileEditor(apath);
    }
    return true;
}

void MainWindow::git()
{
    _logWidget->start("git", QStringList() << "diff" << _project->rootDir().relativeFilePath(_editorTabWidget->currentFilePath()));
}

void MainWindow::makeall()
{
    _editorTabWidget->saveAllEditors();
    _logWidget->start("make",
                      QStringList() << "hex"
                                    << "--no-print-directory"
                                    << "-j" << QString::number(QThread::idealThreadCount() + 1) << "-O");
}

void MainWindow::makeprog()
{
    _editorTabWidget->saveAllEditors();
    _logWidget->start("make",
                      QStringList() << "prog"
                                    << "--no-print-directory"
                                    << "-j" << QString::number(QThread::idealThreadCount() + 1) << "-O");
}

void MainWindow::makesim()
{
    _editorTabWidget->saveAllEditors();
    _logWidget->start("make",
                      QStringList() << "sim"
                                    << "-j" << QString::number(QThread::idealThreadCount() + 1) << "-O");
}

void MainWindow::makeclean()
{
    _logWidget->start("make", QStringList() << "clean");
}

void MainWindow::launchFormat()
{
    _editorTabWidget->saveAllEditors();
    _logWidget->start("clang-format", QStringList() << "-i" << _editorTabWidget->currentFilePath());
}

void MainWindow::updateTitle(Editor *editor)
{
    QString title;
    if (editor)
    {
        title = _project->rootDir().relativeFilePath(editor->filePath());
        if (editor->isModified())
        {
            title.append("*");
        }
        title.append(" | ");
    }
    title.append("(");
    title.append(QDir(_project->rootDir().absolutePath()).dirName());
    title.append(") uDevkit IDE");
    setWindowTitle(title);
}

void MainWindow::updateAction(Editor *editor)
{
    _pasteAction->setEnabled(editor != Q_NULLPTR);
    _searchAction->setEnabled(editor != Q_NULLPTR);
    _replaceAction->setEnabled(editor != Q_NULLPTR);
}

void MainWindow::updateStatus(QString status)
{
    statusBar()->showMessage(status);
}

void MainWindow::showSettings()
{
    SettingsWindow settingsWindow(this);
    settingsWindow.exec();
}

bool MainWindow::event(QEvent *event)
{
    if (event->type() == QEvent::Close)
    {
        if (_editorTabWidget->closeAllEditors() < 0)
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
    for (int i = 0; i < _oldProjects.size() && i < MaxOldProject; i++)
    {
        QString path = _oldProjects[i];
        _oldProjectsActions[i]->setVisible(true);
        _oldProjectsActions[i]->setData(path);
        _oldProjectsActions[i]->setText(QString("&%1. %2").arg(i + 1).arg(path));
        _oldProjectsActions[i]->setStatusTip(tr("Open recent project '") + path + "'");
    }
}

void MainWindow::openRecentFile()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action)
    {
        openDir(action->data().toString());
    }
}

void MainWindow::writeSettings()
{
    QSettings settings(QApplication::organizationName(), QApplication::applicationName());

    // MainWindow position/size/maximized
    settings.beginGroup("MainWindow");
    settings.setValue("geometry", saveGeometry());
    settings.setValue("windowState", saveState());
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
    QSettings settings(QApplication::organizationName(), QApplication::applicationName());

    // MainWindow position/size/maximized
    settings.beginGroup("MainWindow");
    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("windowState").toByteArray());
    settings.endGroup();

    // old projects read
    int size = settings.beginReadArray("projects");
    for (int i = 0; i < size && i < MaxOldProject; ++i)
    {
        settings.setArrayIndex(i);
        QString path = settings.value("path", "").toString();
        if (!_oldProjects.contains(path) && !path.isEmpty() && QDir(path).exists())
        {
            _oldProjects.append(path);
        }
    }
    settings.endArray();

    updateOldProjects();
}

void MainWindow::about()
{
    QMessageBox::about(this,
                       "uDevkit-IDE v0",
                       QString("Copyright (C) 2017-2020 UniSwarm (<a href=\"https://uniswarm.eu\">uniswarm.eu</a>)<br>\
<br>\
This sofware is part of uDevkit distribution. To check for new version, please visit <a href=\"https://github.com/UniSwarm/uDevkit-IDE\">github.com/UniSwarm/uDevkit-IDE</a><br>\
<br>\
Written by <a href=\"https://github.com/sebcaux\">Sébastien CAUX (sebcaux)</a><br>\
<br>\
uDevkit-IDE is a free software: you can redistribute it and/or modify \
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
Build date: ") + __DATE__ + QString(" time: ") +
                           __TIME__ + QString("<br>\
<br>\
uDevkit-IDE use others open libraries :<br>\
- edbee-lib, a code editor widget (code editor) <a href=\"https://github.com/edbee/edbee-lib\">github.com/edbee/edbee-lib</a> [MIT]<br>\
- qhexedit2, a binary editor widget (hex editor) <a href=\"https://github.com/Simsys/qhexedit2\">github.com/Simsys/qhexedit2</a> [GPL]<br>\
- QDarkStyleSheet, a nice dark theme for Qt (dark theme) <a href=\"https://github.com/ColinDuquesnoy/QDarkStyleSheet\">github.com/ColinDuquesnoy/QDarkStyleSheet</a> [MIT]<br>\
- Open Icon Library, free icons lib (mimetype used) <a href=\"https://sourceforge.net/projects/openiconlibrary/\">sourceforge.net/projects/openiconlibrary/</a> [CC]<br>\
- icons8, online icons library (menu and tool buttons) <a href=\"https://icons8.com\">icons8.com</a> [Free for Open]<br>\
"));
}
