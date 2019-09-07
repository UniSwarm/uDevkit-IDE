#ifndef HTMLEDITOR_H
#define HTMLEDITOR_H

#include "codeeditor.h"

#ifndef NOWEBKIT
    #if !defined(WEBENGINE) && !defined(WEBKIT)
        #define NOWEBKIT
    #endif
#endif

#ifdef WEBENGINE
class QWebEngineView;
#endif
#ifdef WEBKIT
class QWebView;
#endif
#ifdef NOWEBKIT
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
    virtual void updatePreview();
    int openFileData(const QString &filePath);

protected:
    void createPreview();

#ifdef WEBENGINE
    QWebEngineView *_htmlPreview;
#endif
#ifdef WEBKIT
    QWebView *_htmlPreview;
#endif
#ifdef NOWEBKIT
    QTextBrowser *_htmlPreview;
#endif
};

#endif // HTMLEDITOR_H
