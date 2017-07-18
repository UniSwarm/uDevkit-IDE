#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QWidget>
#include "edbee/texteditorwidget.h"

class CodeEditor : public QWidget
{
    Q_OBJECT
public:
    explicit CodeEditor(QWidget *parent = 0);

signals:

public slots:
    int openFile(const QString &fileName);

protected:
    edbee::TextEditorWidget* _editorWidget;
};

#endif // CODEEDITOR_H
