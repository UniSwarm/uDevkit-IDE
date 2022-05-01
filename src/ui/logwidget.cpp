/**
 ** This file is part of the uDevkit-IDE project.
 ** Copyright 2017-2021 UniSwarm
 **
 ** This program is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU General Public License as published by
 ** the Free Software Foundation, either version 3 of the License, or
 ** (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 ** GNU General Public License for more details.
 **
 ** You should have received a copy of the GNU General Public License
 ** along with this program. If not, see <http://www.gnu.org/licenses/>.
 **/

#include "logwidget.h"

#include "project/project.h"
#include "settings/settingsmanager.h"

#include <QDebug>
#include <QDir>
#include <QRegularExpression>
#include <QStandardPaths>
#include <QTextStream>

LogWidget::LogWidget(Project *project, QWidget *parent)
    : QTextBrowser(parent)
    , _project(project)
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

    connect(_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, &LogWidget::finishProcess);
    connect(_process, &QProcess::errorOccurred, this, &LogWidget::errorProcess);
    connect(_process, &QProcess::readyReadStandardOutput, this, &LogWidget::readProcess);
    connect(_process, &QProcess::readyReadStandardError, this, &LogWidget::readProcess);
    _process->setWorkingDirectory(_project->rootPath());

    connect(this, &LogWidget::anchorClicked, this, &LogWidget::anchorClick);
}

LogWidget::~LogWidget()
{
    _process->terminate();
    delete _process;
}

void LogWidget::start(const QString &program, const QStringList &arguments)
{
    _process->terminate();
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

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    stream.setEncoding(QStringConverter::Utf8);
#else
    stream.setCodec("UTF-8");
#endif
    QRegularExpression colorReg("(\u001b)\\[([0-9]+)m");
    QRegularExpression colorRstReg("(\u001b\\(B\u001b\\[m|\u001b\\[0;10m)");
    QRegularExpression linkReg(R"(([\-\._a-zA-Z/\\0-9]+\.[a-zA-Z]+)(:[0-9]+)(:[0-9]+)*)");

    while (!stream.atEnd())
    {
        QString stringRead = stream.readLine();
        QString errorFormat = error ? " class=\"color31\"" : "";
        stringRead = "<br/><span" + errorFormat + ">" + stringRead.toHtmlEscaped() + "</span>";
        stringRead.replace(" ", "&nbsp;");
        stringRead.replace(colorReg, R"(</span><span class="color\2">)");
        stringRead.replace(colorRstReg, "</span><span>");  // reset color
        stringRead.replace("<span></span>", "");

        stringRead.replace(linkReg, R"(<a href="\1\2\3">\1\2\3</a>)");

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
    data.append(_process->errorString().toUtf8());
    parseOutput(data, true);
}

void LogWidget::finishProcess(int errorCode, QProcess::ExitStatus exitStatus)
{
    readProcess();
    moveCursor(QTextCursor::End);
    if (exitStatus == QProcess::NormalExit && errorCode == 0)
    {
        insertHtml(QString("<br/><i style=\"color: green\">&gt; %1 finished with code %2</i>").arg(_process->program()).arg(errorCode));
    }
    else
    {
        insertHtml(QString("<br/><i style=\"color: red\">&gt; %1 crashed with code %2</i>").arg(_process->program()).arg(errorCode));
    }
    moveCursor(QTextCursor::End);
}

void LogWidget::anchorClick(const QUrl &link)
{
    emit openFileRequested(link.toString(QUrl::None));
}
