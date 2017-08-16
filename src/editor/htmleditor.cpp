#include "htmleditor.h"

#include <QDebug>

#include <QBoxLayout>
#include <QWebEngineView>
#include "edbee/texteditorwidget.h"
#include "edbee/models/textdocument.h"

#include <QDir>

HtmlEditor::HtmlEditor(Project *project, QWidget *parent)
    : CodeEditor (project, parent)
{
    _htmlPreview = new QWebEngineView();
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
    QByteArray data;
    data.append(_editorWidget->textDocument()->text());
    _htmlPreview->setContent(data, "text/html", QUrl::fromLocalFile(_filePath));
    _editorWidget->setFocus();
}

int HtmlEditor::openFileData(const QString &filePath)
{
    CodeEditor::openFileData(filePath);
    updatePreview();
}
