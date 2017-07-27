#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include "editor.h"
#include "edbee/texteditorwidget.h"

class CodeEditor : public Editor
{
    Q_OBJECT
public:
    explicit CodeEditor(QWidget *parent = 0);

    bool isModified() const;

    Type type() const {return Code;}
protected:
    edbee::TextEditorWidget* _editorWidget;

    int openFileData(const QString &filePath);
    int saveFileData(const QString &filePath=QString());

protected slots:
    void modificationAppend();

private:
    static bool initialized;
};

#endif // CODEEDITOR_H
