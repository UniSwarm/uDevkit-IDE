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

#include "htmleditor.h"

#include <QDebug>

#ifdef WEBENGINE
#    include <QWebEngineView>
#endif
#ifdef WEBKIT
#    include <QWebView>
#endif
#ifdef NOWEBKIT
#    include <QTextBrowser>
#endif

#include "edbee/models/textdocument.h"
#include "edbee/texteditorwidget.h"
#include <QBoxLayout>

#include <QDir>

HtmlEditor::HtmlEditor(Project *project, QWidget *parent)
    : CodeEditor(project, parent)
{
    createPreview();
    connect(_editorWidget->textDocument(), &edbee::TextDocument::textChanged, this, &HtmlEditor::updatePreview);
}

QWidget *HtmlEditor::previewWidget() const
{
    return _htmlPreview;
}

bool HtmlEditor::hasPreview() const
{
    return true;
}

void HtmlEditor::updatePreview()
{
#ifndef NOWEBKIT
    QByteArray data;
    data.append(_editorWidget->textDocument()->text());
    _htmlPreview->setContent(data, "text/html", QUrl::fromLocalFile(_filePath));
    _editorWidget->setFocus();
#else
    _htmlPreview->setHtml(_editorWidget->textDocument()->text());
#endif
}

int HtmlEditor::openFileData(const QString &filePath)
{
    int res = CodeEditor::openFileData(filePath);
    updatePreview();
    return res;
}

void HtmlEditor::createPreview()
{
#ifdef WEBENGINE
    _htmlPreview = new QWebEngineView();
#endif
#ifdef WEBKIT
    _htmlPreview = new QWebView();
#endif
#ifdef NOWEBKIT
    _htmlPreview = new QTextBrowser();
    _htmlPreview->setReadOnly(true);
#endif
}
