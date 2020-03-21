#include "htmleditor.h"

#include <QDebug>

#ifdef WEBENGINE
#include <QWebEngineView>
#endif
#ifdef WEBKIT
#include <QWebView>
#endif
#ifdef NOWEBKIT
#include <QTextBrowser>
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
