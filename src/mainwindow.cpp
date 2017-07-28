#include "mainwindow.h"

#include <QBoxLayout>
#include <QApplication>
#include <QDesktopWidget>
#include <QSplitter>
#include <QAction>

MainWindow::MainWindow(Project *project, QWidget *parent) :
    QMainWindow(parent), _project(project)
{
    _process = nullptr;

    if(!_project)
        _project = new Project(QDir::home().canonicalPath());

    _editorTabWidget = new EditorTabWidget();
    //_editorTabWidget->addFileEditor(QApplication::applicationDirPath()+"/../../rtprog/README.md");
    //_editorTabWidget->setFocus();

    setCentralWidget(_editorTabWidget);
    connect(_editorTabWidget, &EditorTabWidget::editorChange, this, &MainWindow::updateTitle);
    connect(_editorTabWidget, &EditorTabWidget::currentEditorModified, this, &MainWindow::updateTitle);
    resize(QDesktopWidget().availableGeometry(this).size() * 0.7);

    createDocks();
    registerAction();
}

MainWindow::~MainWindow()
{
}

void MainWindow::createDocks()
{
    setCorner(Qt::TopLeftCorner, Qt::LeftDockWidgetArea);
    setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);
    setTabPosition(Qt::LeftDockWidgetArea, QTabWidget::North);

    _fileProjectDock = new QDockWidget("project", this);
    QWidget *fileProjectContent = new QWidget(_fileProjectDock);
    QLayout *fileProjectLayout = new QVBoxLayout();
    _fileProjectWidget = new FileProjectWidget(_project);
    fileProjectLayout->addWidget(_fileProjectWidget);
    connect(_fileProjectWidget, &FileProjectWidget::doubleClickedFile, _editorTabWidget, &EditorTabWidget::addFileEditor);
    fileProjectContent->setLayout(fileProjectLayout);
    _fileProjectDock->setWidget(fileProjectContent);
    addDockWidget(Qt::LeftDockWidgetArea, _fileProjectDock);

    _logDock = new QDockWidget("log", this);
    QWidget *logContent = new QWidget(_logDock);
    QLayout *logLayout = new QVBoxLayout();
    _logWidget = new QTextEdit();
    _logWidget->document()->setDefaultStyleSheet("p{margin: 0;}");
    logLayout->addWidget(_logWidget);
    logContent->setLayout(logLayout);
    _logDock->setWidget(logContent);
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

void MainWindow::registerAction()
{
    QAction *action;
    /*action = new QAction(QString("git"), this);
    action->setShortcut(QKeySequence("F2"));
    addAction(action);
    connect(action, &QAction::triggered, this, &MainWindow::git);*/

    action = new QAction(QString("next tab"), this);
    action->setShortcut(QKeySequence::NextChild);
    addAction(action);
    connect(action, &QAction::triggered, _editorTabWidget, &EditorTabWidget::nextTab);

    action = new QAction(QString("search"), this);
    action->setShortcut(QKeySequence::QKeySequence::Find);
    addAction(action);
    connect(action, &QAction::triggered, _searchReplaceWidget, &SearchReplaceWidget::activate);
}

void MainWindow::git()
{
    _process = new QProcess(this);
    connect(_process, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(readProcess()));
    _process->setWorkingDirectory(QApplication::applicationDirPath());
    _process->start("git", QStringList()<<"status");
}

void MainWindow::readProcess()
{
    QString html;
    QByteArray dataRead;

    while(_process->canReadLine())
    {
        dataRead = _process->readLine();
        QString stringRead = QString::fromLocal8Bit(dataRead);
        stringRead.replace(QRegExp("\\x001b\\[([0-9]+)m"),"");
        stringRead = stringRead.toHtmlEscaped();
        html.append("<p>" + stringRead + "</p>");
    }
    dataRead = _process->readAllStandardError();
    if(!dataRead.isEmpty())
        html.append("<p><span style=\"color: red\">"+QString::fromLocal8Bit(dataRead)+"</span></p>");
    _logWidget->append(html);

    _process->deleteLater();
    _process = nullptr;
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
