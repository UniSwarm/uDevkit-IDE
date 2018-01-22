#include "logwidget.h"

#include "project/project.h"
#include "settings/settingsmanager.h"

#include <QDebug>
#include <QTextStream>
#include <QStandardPaths>
#include <QDir>
#include <QRegularExpression>

LogWidget::LogWidget(Project *project, QWidget *parent)
    : QTextBrowser(parent), _project(project)
{
    setTabChangesFocus(true);
    setReadOnly(true);
    setOpenLinks(false);
    _color = "30";

    _process = new QProcess(this);

#ifdef WIN32
    QFont font("Consolas");
#else
    QFont font("monospace");
#endif
    font.setStyleHint(QFont::Monospace);
    setFont(font);

    connect(_process, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(readProcess()));
    connect(_process, SIGNAL(error(QProcess::ProcessError)), this, SLOT(errorProcess()));
    connect(_process, SIGNAL(readyReadStandardOutput()), this, SLOT(readProcess()));
    connect(_process, SIGNAL(readyReadStandardError()), this, SLOT(readProcess()));
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
    QString command = "> " + program + " " + arguments.join(' ');
    insertHtml("<i style=\"color: green\">" + command.toHtmlEscaped() + "</i>");

    QProcessEnvironment env(QProcessEnvironment::systemEnvironment());
#if defined(Q_OS_WIN)
    QChar listSep = ';';
#else
    QChar listSep = ':';
#endif
    QString makePath = rtset()->setting("tools/make/path").toString();
    if (!makePath.isEmpty())
    {
        makePath = QDir::toNativeSeparators(makePath);
        env.insert("PATH", makePath + listSep + env.value("PATH"));
    }
    env.insert("TERM", "xterm");
    _process->setProcessEnvironment(env);

    QString path = QStandardPaths::findExecutable(program, env.value("PATH").split(listSep));
    if (path.isEmpty())
    {
        insertHtml("<br/><i style=\"color: red\">" + tr("Cannot find command '%1'").arg(program).toHtmlEscaped() + "</i>");
        return;
    }

    document()->setDefaultStyleSheet("\
.color30 { color: black; }\n\
.color31 { color: red; }\n\
.color32 { color: green; }\n\
.color33 { color: yellow; }\n\
.color34 { color: blue; }\n\
.color35 { color: magenta; }\n\
.color36 { color: cyan; }\n\
.color37 { color: white; }\n\
a { color: white; }\n\
");
    _process->setWorkingDirectory(_project->rootPath());
    _process->start(path, arguments);
}

void LogWidget::parseOutput(QByteArray data, bool error)
{
    QString html;
    QTextStream stream(&data);
    stream.setCodec("UTF-8");

    QRegularExpression colorReg("(\\x001b|)\\[([0-9]+)m");
    QRegularExpression colorRstReg("(\\x001b\\(B\\x001b\\[m|\\[0;10m)");
    QRegularExpression linkReg("([\\-\\._a-zA-Z/\\\\0-9]+\\.[a-zA-Z]+)(:[0-9]+)*(:[0-9]+)*");

    while (!stream.atEnd())
    {
        QString stringRead = stream.readLine();
        QString errorFormat = error ? " class=\"color31\"" : "";
        stringRead = "<br/><span" + errorFormat + ">" + stringRead.toHtmlEscaped() + "</span>";
        stringRead.replace(" ","&nbsp;");
        stringRead.replace(colorReg,"</span><span class=\"color\\2\">");
        stringRead.replace(colorRstReg,"</span><span>"); // reset color
        stringRead.replace("<span></span>","");

        stringRead.replace(linkReg,"<a href=\"\\1\\2\\3\">\\1\\2\\3</a>");

        html.append(stringRead);
    }
    moveCursor(QTextCursor::End);
    insertHtml(html);
    moveCursor(QTextCursor::End);
}

void LogWidget::readProcess()
{
    parseOutput(_process->readAllStandardOutput(), false);
    parseOutput(_process->readAllStandardError(), true);
}

void LogWidget::errorProcess()
{
    QByteArray data;
    data.append(_process->errorString());
    parseOutput(data, true);
}

void LogWidget::anchorClick(const QUrl &link)
{
    emit openFileRequested(link.toString(QUrl::None));
}
