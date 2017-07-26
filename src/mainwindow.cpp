#include "mainwindow.h"

#include <QBoxLayout>
#include <QApplication>
#include <QDesktopWidget>
#include <QSplitter>
#include <QAction>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    QSplitter *spitter = new QSplitter(this);

    _projet = new Project(QApplication::applicationDirPath()+"/../../rtprog/");

    _fileView = new FileProjectWidget(_projet);
    spitter->addWidget(_fileView);

    _editorTabWidget = new EditorTabWidget();
    _editorTabWidget->addFileEditor(QApplication::applicationDirPath()+"/../../rtprog/README.md");
    spitter->addWidget(_editorTabWidget);
    _editorTabWidget->setFocus();

    connect(_fileView, &FileProjectWidget::doubleClickFile, _editorTabWidget, &EditorTabWidget::addFileEditor);

    spitter->setSizes(QList<int>()<<100<<500);

    setCentralWidget(spitter);
    resize(QDesktopWidget().availableGeometry(this).size() * 0.7);

    setCorner(Qt::TopLeftCorner, Qt::LeftDockWidgetArea);
    setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);
    setTabPosition(Qt::LeftDockWidgetArea, QTabWidget::North);
    _dockLog = new QDockWidget("log", this);
    QWidget *logContent = new QWidget(_dockLog);
    QLayout *logLayout = new QVBoxLayout();
    _logWidget = new QTextEdit();
    _logWidget->document()->setDefaultStyleSheet("p{margin: 0;}");
    logLayout->addWidget(_logWidget);
    logContent->setLayout(logLayout);
    _dockLog->setWidget(logContent);
    addDockWidget(Qt::BottomDockWidgetArea, _dockLog);

    _process = nullptr;

    registerAction();
}

MainWindow::~MainWindow()
{
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
