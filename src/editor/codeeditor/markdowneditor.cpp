#include "markdowneditor.h"

#ifdef WEBENGINE
#include <QWebEngineView>
#endif
#ifdef WEBKIT
#include <QWebView>
#endif
#ifdef NOWEBKIT
#include <QTextBrowser>
#endif

#include "edbee/texteditorwidget.h"
#include "edbee/models/textdocument.h"

#include "markdown.h"
//#include "mkdio.h"

#include <QDebug>

MarkdownEditor::MarkdownEditor(Project *project, QWidget *parent)
    : HtmlEditor (project, parent)
{
    connect(_editorWidget->textDocument(), &edbee::TextDocument::textChanged, this, &MarkdownEditor::updatePreview);
}

void MarkdownEditor::updatePreview()
{
    QByteArray data;
    char *ptData, *ptCssData;

    mkd_flag_t mdFlags = MKD_NOHEADER;
    QByteArray textIn = _editorWidget->textDocument()->text().toLocal8Bit();
    char *text = textIn.data();

    Document *mdDoc = gfm_string(text, strlen(text), mdFlags);
    if (!mdDoc)
        return;

    //mkd_with_html5_tags();
    mkd_compile(mdDoc, mdFlags);
    //mkd_generatetoc();
    mkd_css(mdDoc, &ptCssData);
    QByteArray cssData(ptCssData);
    data.append(cssData);
    //qDebug()<<cssData;

    mkd_document(mdDoc, &ptData);
    QByteArray mdData(ptData);
    data.append(mdData);
    //qDebug()<<mdData;

    mkd_cleanup(mdDoc);

#ifndef NOWEBKIT
    _htmlPreview->setContent(data, "text/html", QUrl::fromLocalFile(_filePath));
    _editorWidget->setFocus();
#else
    _htmlPreview->setHtml(_editorWidget->textDocument()->text());
#endif
}
