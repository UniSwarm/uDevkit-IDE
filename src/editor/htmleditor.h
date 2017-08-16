#ifndef HTMLEDITOR_H
#define HTMLEDITOR_H

#include "codeeditor.h"

class QWebEngineView;

class HtmlEditor : public CodeEditor
{
    Q_OBJECT
public:
    HtmlEditor(Project *project, QWidget *parent = 0);

    // Editor interface
public:
    virtual QWidget *previewWidget() const;
    virtual bool hasPreview() const;

protected slots:
    void updatePreview();
    int openFileData(const QString &filePath);

protected:
    QWebEngineView *_htmlPreview;
};

#endif // HTMLEDITOR_H
