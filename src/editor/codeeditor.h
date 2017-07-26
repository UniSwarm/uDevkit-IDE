#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include "editor.h"
#include "edbee/texteditorwidget.h"

class CodeEditor : public Editor
{
    Q_OBJECT
public:
    explicit CodeEditor(QWidget *parent = 0);

    bool modified() const;

signals:

public slots:
    int openFile(const QString &filePath);

protected:
    edbee::TextEditorWidget* _editorWidget;

private:
    static bool initialized;
};

#endif // CODEEDITOR_H
