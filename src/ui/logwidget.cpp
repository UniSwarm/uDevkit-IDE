#include "logwidget.h"

#include "project/project.h"

#include <QDebug>

LogWidget::LogWidget(Project *project, QWidget *parent)
    : QTextBrowser(parent), _project(project)
{
    setTabChangesFocus(true);
    setReadOnly(true);
    setOpenLinks(false);
    _color = "30";

    _process = new QProcess(this);

    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("TERM", "xterm");
    _process->setProcessEnvironment(env);

    connect(_process, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(readProcess()));
    connect(_process, SIGNAL(readyReadStandardOutput()), this, SLOT(readProcess()));
    _process->setWorkingDirectory(_project->rootPath());

    connect(this, SIGNAL(anchorClicked(QUrl)), this, SLOT(anchorClick(QUrl)));
}

LogWidget::~LogWidget()
{
    _process->terminate();
    delete _process;
}

void LogWidget::start(const QString &program, const QStringList &arguments)
{
    clear();
    _process->start(program, arguments);
}

void LogWidget::readProcess()
{
    QString html;
    QByteArray dataRead;

    while(_process->canReadLine())
    {
        dataRead = _process->readLine();
        QString stringRead = QString::fromLocal8Bit(dataRead);
        stringRead = "<br/><span>" + stringRead.toHtmlEscaped() + "</span>";
        stringRead.replace("\n","");
        stringRead.replace(QRegExp("\\x001b\\[([0-9]+)m"),"</span><span class=\"color\\1\">");
        stringRead.replace(QRegExp("\\x001b\\(B\\x001b\\[m"),"</span><span>");
        stringRead.replace("<span></span>","");

        stringRead.replace(QRegExp("([\\-\\._a-zA-Z/\\\\0-9]+\\.[a-zA-Z]+)"),"<a href=\"\\1\">\\1</a>");

        html.append(stringRead);
    }
    dataRead = _process->readAllStandardError();
    if(!dataRead.isEmpty())
        html.append("<br/><span class=\"color31\">"+QString::fromLocal8Bit(dataRead).replace("\n","")+"</span>");

    document()->setDefaultStyleSheet("\
* {font-family: monospace;}\n\
.color30 { color: black; }\n\
.color31 { color: red; }\n\
.color32 { color: green; }\n\
.color33 { color: yellow; }\n\
.color34 { color: blue; }\n\
.color35 { color: magenta; }\n\
.color36 { color: cyan; }\n\
.color37 { color: white; }\n\
");
    moveCursor(QTextCursor::End);
    insertHtml(html);
    moveCursor(QTextCursor::End);
}

void LogWidget::anchorClick(const QUrl &link)
{
    QString filePath = _project->make()->resolveFilePath(link.toString(QUrl::None));
    if (!filePath.isEmpty())
        emit openFileRequested(filePath);
}
