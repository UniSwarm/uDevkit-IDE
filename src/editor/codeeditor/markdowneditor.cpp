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

#include "markdowneditor.h"

#ifdef WEBENGINE
#    include <QWebEngineView>
#endif
#ifdef WEBKIT
#    include <QWebView>
#endif
#ifdef NOWEBKIT
#    include <QTextBrowser>
#endif

#include <QtConcurrent/QtConcurrent>

#include "edbee/models/textdocument.h"
#include "edbee/texteditorwidget.h"

#include "maddy/qmaddy.h"

MarkdownEditor::MarkdownEditor(Project *project, QWidget *parent)
    : HtmlEditor(project, parent)
{
    connect(&watcher, &QFutureWatcher<QByteArray>::finished, this, &MarkdownEditor::showHtml);
}

void MarkdownEditor::updatePreview()
{
    launchRender();
}

QByteArray renderProcess(const QByteArray &textIn)
{
    maddy::QMaddy markdownParser;
    markdownParser.setCss("https://sindresorhus.com/github-markdown-css/github-markdown.css");
    QByteArray dataOut = markdownParser.toHtml(textIn);
    return dataOut;
}

void MarkdownEditor::launchRender()
{
    QByteArray textIn = _editorWidget->textDocument()->text().toLocal8Bit();
    QFuture<QByteArray> future = QtConcurrent::run(renderProcess, textIn);
    watcher.setFuture(future);
}

void MarkdownEditor::showHtml()
{
    const QByteArray dataOut = watcher.future().result();
#ifndef NOWEBKIT
    _htmlPreview->setContent(dataOut, "text/html", QUrl::fromLocalFile(_filePath));
    _editorWidget->setFocus();
#else
    _htmlPreview->setHtml(QString::fromUtf8(dataOut));
#endif
}
