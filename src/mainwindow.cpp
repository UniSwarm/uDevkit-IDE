#include "mainwindow.h"

#include <QBoxLayout>
#include <QApplication>
#include <QDesktopWidget>
#include <QSplitter>
#include <QAction>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    _process = nullptr;

    _projet = new Project(QApplication::applicationDirPath()+"/../../rtprog/");

    _editorTabWidget = new EditorTabWidget();
    _editorTabWidget->addFileEditor(QApplication::applicationDirPath()+"/../../rtprog/README.md");
    //_editorTabWidget->setFocus();

    setCentralWidget(_editorTabWidget);
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

    _dockFileProject = new QDockWidget("project", this);
    QWidget *fileProjectContent = new QWidget(_dockFileProject);
    QLayout *fileProjectLayout = new QVBoxLayout();
    _fileProjectWidget = new FileProjectWidget(_projet);
    fileProjectLayout->addWidget(_fileProjectWidget);
    connect(_fileProjectWidget, &FileProjectWidget::doubleClickFile, _editorTabWidget, &EditorTabWidget::addFileEditor);
    fileProjectContent->setLayout(fileProjectLayout);
    _dockFileProject->setWidget(fileProjectContent);
    addDockWidget(Qt::LeftDockWidgetArea, _dockFileProject);

    _dockLog = new QDockWidget("log", this);
    QWidget *logContent = new QWidget(_dockLog);
    QLayout *logLayout = new QVBoxLayout();
    _logWidget = new QTextEdit();
    _logWidget->document()->setDefaultStyleSheet("p{margin: 0;}");
    logLayout->addWidget(_logWidget);
    logContent->setLayout(logLayout);
    _dockLog->setWidget(logContent);
    addDockWidget(Qt::BottomDockWidgetArea, _dockLog);
}

void MainWindow::registerAction()
{
    QAction *action;
    action = new QAction(QString("git"), this);
    action->setShortcut(QKeySequence("F2"));
    addAction(action);
    connect(action, &QAction::triggered, this, &MainWindow::git);
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
