#ifndef HTMLEDITOR_H
#define HTMLEDITOR_H

#include "codeeditor.h"

#ifndef NO_WEBENGINE
class QWebEngineView;
#else
class QTextBrowser;
#endif

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
#ifndef NO_WEBENGINE
    QWebEngineView *_htmlPreview;
#else
    QTextBrowser *_htmlPreview;
#endif
};

#endif // HTMLEDITOR_H
